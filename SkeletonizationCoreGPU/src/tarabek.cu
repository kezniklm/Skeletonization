#include "SkeletonizationCoreGPU/tarabek.cuh"

namespace skeletonizer::gpu::algorithms
{
	void tarabek::apply(cv::Mat& binary_image) const
	{
		cv::cuda::GpuMat gpu_src(binary_image);
		cv::cuda::GpuMat gpu_dst(binary_image.size(), gpu_src.type());

		const dim3 block(block_dimension_x, block_dimension_y);
		const dim3 grid(
			(gpu_src.cols + block.x - 1) / block.x,
			(gpu_src.rows + block.y - 1) / block.y);

		int host_changed = 0;
		int* device_changed = nullptr;

		cudaMalloc(&device_changed, sizeof(int));

		cv::cuda::GpuMat* src = &gpu_src;
		cv::cuda::GpuMat* dst = &gpu_dst;

		do
		{
			cudaMemset(device_changed, 0, sizeof(int));

			tarabek_iteration(
				*src, *dst,
				true,
				device_changed,
				grid, block, halo);

			std::swap(src, dst);

			tarabek_iteration(
				*src, *dst,
				false,
				device_changed,
				grid, block, halo);

			std::swap(src, dst);

			cudaMemcpy(&host_changed,
			           device_changed,
			           sizeof(int),
			           cudaMemcpyDeviceToHost);
		}
		while (host_changed != 0);

		cudaFree(device_changed);

		tarabek_postprocessing(*src, *dst, grid, block, halo);
		std::swap(src, dst);

		src->download(binary_image);

		clear_border(binary_image);

		cudaDeviceSynchronize();
	}
}

__global__ void tarabek_iteration_kernel(
	const cv::cuda::PtrStep<uchar> src,
	cv::cuda::PtrStep<uchar> dst,
	const int num_rows,
	const int num_cols,
	const bool first_pass,
	int* d_changed,
	const int halo
)
{
	extern __shared__ uchar shared_tile[];

	const int local_x = threadIdx.x;
	const int local_y = threadIdx.y;
	const int global_x = global_index_x(blockIdx.x, local_x, blockDim.x);
	const int global_y = global_index_y(blockIdx.y, local_y, blockDim.y);

	const int shared_stride = blockDim.x + 2 * halo;

	shared_tile[shared_index(local_x, local_y, shared_stride, halo)] = load_center_pixel(
		src, global_x, global_y, num_cols, num_rows);

	load_halo_edges(shared_tile, src, global_x, global_y, num_cols, num_rows, shared_stride, local_x, local_y, halo,
	                blockDim);
	load_halo_corners(shared_tile, src, global_x, global_y, num_cols, num_rows, shared_stride, local_x, local_y, halo,
	                  blockDim);

	__syncthreads();

	if (is_out_of_bounds(global_x, global_y, num_cols, num_rows))
	{
		return;
	}

	if (is_border_pixel(global_x, global_y, num_cols, num_rows, halo))
	{
		dst(global_y, global_x) = src(global_y, global_x);
		return;
	}

	const auto p1 = shared_tile[shared_index(local_x, local_y, shared_stride, halo)];
	const auto p2 = shared_tile[shared_index(local_x, local_y - 1, shared_stride, halo)];
	const auto p3 = shared_tile[shared_index(local_x + 1, local_y - 1, shared_stride, halo)];
	const auto p4 = shared_tile[shared_index(local_x + 1, local_y, shared_stride, halo)];
	const auto p5 = shared_tile[shared_index(local_x + 1, local_y + 1, shared_stride, halo)];
	const auto p6 = shared_tile[shared_index(local_x, local_y + 1, shared_stride, halo)];
	const auto p7 = shared_tile[shared_index(local_x - 1, local_y + 1, shared_stride, halo)];
	const auto p8 = shared_tile[shared_index(local_x - 1, local_y, shared_stride, halo)];
	const auto p9 = shared_tile[shared_index(local_x - 1, local_y - 1, shared_stride, halo)];

	if (p1 == background)
	{
		dst(global_y, global_x) = background;
		return;
	}

	const auto a = (p2 == 0 && p3 == 1) +
		(p3 == 0 && p4 == 1) +
		(p4 == 0 && p5 == 1) +
		(p5 == 0 && p6 == 1) +
		(p6 == 0 && p7 == 1) +
		(p7 == 0 && p8 == 1) +
		(p8 == 0 && p9 == 1) +
		(p9 == 0 && p2 == 1);

	const auto b = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;

	const auto step_condition_c = first_pass ? p2 * p4 * p6 : p2 * p4 * p8;
	const auto step_condition_d = first_pass ? p4 * p6 * p8 : p2 * p6 * p8;

	if (a != 1 || b < 2 || b > 6 || step_condition_c != 0 || step_condition_d != 0)
	{
		dst(global_y, global_x) = skeleton;
		return;
	}

	if (b == 2)
	{
		const auto bn4 = p2 + p4 + p6 + p8;

		const auto an4 = (p2 == 0 && p4 == 1) +
			(p4 == 0 && p6 == 1) +
			(p6 == 0 && p8 == 1) +
			(p8 == 0 && p2 == 1);

		const auto bnd = p3 + p5 + p7 + p9;

		const auto AND = (p3 == 0 && p5 == 1) +
			(p5 == 0 && p7 == 1) +
			(p7 == 0 && p9 == 1) +
			(p9 == 0 && p3 == 1);

		if (bn4 == 3 && an4 == 2 && bnd == 4 && AND == 2)
		{
			dst(global_y, global_x) = skeleton;
			return;
		}
	}

	if (b == 3 && a == 1 && p4 == foreground && p5 == foreground && p6 == foreground)
	{
		dst(global_y, global_x) = skeleton;
		return;
	}

	dst(global_y, global_x) = background;
	atomicExch(d_changed, 1);
}

extern inline void tarabek_iteration(
	const cv::cuda::GpuMat& src,
	const cv::cuda::GpuMat& dst,
	const bool first_pass,
	int* d_changed,
	dim3 grid,
	dim3 block,
	const int halo)
{
	const auto shared_memory = compute_shared_mem_size(block, halo);

	tarabek_iteration_kernel<<<grid, block, shared_memory>>>(src, dst, src.rows, src.cols, first_pass,
	                                                         d_changed, halo);
}

struct mask_bits
{
	uint16_t fixed_mask;
	uint16_t fixed_value;
	uint16_t n4_mask;
	uint16_t nd_mask;
	uint8_t n4_marked_count;
};

__constant__ mask_bits d_templates_a[32] = {
	{510, 254, 170, 68, 4}, {507, 443, 170, 257, 4}, {447, 443, 170, 261, 4}, {255, 254, 170, 69, 4},
	{510, 254, 170, 68, 4}, {507, 443, 170, 257, 4}, {447, 443, 170, 261, 4}, {255, 254, 170, 69, 4},
	{510, 254, 170, 68, 4}, {507, 443, 170, 257, 4}, {447, 443, 170, 261, 4}, {255, 254, 170, 69, 4},
	{510, 254, 170, 68, 4}, {507, 443, 170, 257, 4}, {447, 443, 170, 261, 4}, {255, 254, 170, 69, 4},
	{510, 254, 170, 68, 4}, {507, 443, 170, 257, 4}, {447, 443, 170, 261, 4}, {255, 254, 170, 69, 4},
	{510, 254, 170, 68, 4}, {507, 443, 170, 257, 4}, {447, 443, 170, 261, 4}, {255, 254, 170, 69, 4},
	{510, 254, 170, 68, 4}, {507, 443, 170, 257, 4}, {447, 443, 170, 261, 4}, {255, 254, 170, 69, 4},
	{510, 254, 170, 68, 4}, {507, 443, 170, 257, 4}, {447, 443, 170, 261, 4}, {255, 254, 170, 69, 4}
};

__constant__ mask_bits d_templates_b[16] = {
	{511, 443, 170, 325, 4}, {511, 254, 170, 68, 4}, {511, 509, 168, 325, 3}, {511, 479, 138, 325, 3},
	{511, 443, 170, 325, 4}, {511, 254, 170, 68, 4}, {511, 509, 168, 325, 3}, {511, 479, 138, 325, 3},
	{511, 443, 170, 325, 4}, {511, 254, 170, 68, 4}, {511, 509, 168, 325, 3}, {511, 479, 138, 325, 3},
	{511, 443, 170, 325, 4}, {511, 254, 170, 68, 4}, {511, 509, 168, 325, 3}, {511, 479, 138, 325, 3}
};

__device__ __forceinline__ bool aux_conditions_met_bits_device(const mask_bits& mask_bits,
                                                               const uint16_t neigh_bits) noexcept
{
	const auto n4_marked_mask = static_cast<uint16_t>(mask_bits.fixed_mask & mask_bits.n4_mask);

	const auto n4_marked_count = __popc(n4_marked_mask);

	if (n4_marked_count >= 2)
	{
		const auto n4_background_mask = static_cast<uint16_t>(n4_marked_mask & ~neigh_bits);

		const auto n4_background_count = __popc(n4_background_mask);

		if (n4_background_count == n4_marked_count)
		{
			return true;
		}
	}

	if (n4_marked_count >= 1)
	{
		const auto n4_background_mask = static_cast<uint16_t>(n4_marked_mask & ~neigh_bits);

		const auto n4_bg_count = __popc(n4_background_mask);

		if (n4_bg_count == 1)
		{
			const auto nd_marked_mask = static_cast<uint16_t>(mask_bits.fixed_mask & mask_bits.nd_mask);

			const auto nd_foreground_mask = static_cast<uint16_t>(nd_marked_mask & neigh_bits);

			const auto nd_foreground_count = __popc(nd_foreground_mask);

			if (nd_foreground_count >= 1)
			{
				return true;
			}
		}
	}

	return false;
}

__device__ __forceinline__ uint16_t compute_neigh_bits_shared(const uchar* shared_tile, const int shared_stride,
                                                              const int local_x, const int local_y,
                                                              const int halo) noexcept
{
	return static_cast<uint16_t>(
		static_cast<uint16_t>(shared_tile[shared_index(local_x - 1, local_y - 1, shared_stride, halo)] ? 1u : 0u) << 0 |
		static_cast<uint16_t>(shared_tile[shared_index(local_x, local_y - 1, shared_stride, halo)] ? 1u : 0u) << 1 |
		static_cast<uint16_t>(shared_tile[shared_index(local_x + 1, local_y - 1, shared_stride, halo)] ? 1u : 0u) << 2 |
		static_cast<uint16_t>(shared_tile[shared_index(local_x - 1, local_y, shared_stride, halo)] ? 1u : 0u) << 3 |
		static_cast<uint16_t>(shared_tile[shared_index(local_x, local_y, shared_stride, halo)] ? 1u : 0u) << 4 |
		static_cast<uint16_t>(shared_tile[shared_index(local_x + 1, local_y, shared_stride, halo)] ? 1u : 0u) << 5 |
		static_cast<uint16_t>(shared_tile[shared_index(local_x - 1, local_y + 1, shared_stride, halo)] ? 1u : 0u) << 6 |
		static_cast<uint16_t>(shared_tile[shared_index(local_x, local_y + 1, shared_stride, halo)] ? 1u : 0u) << 7 |
		static_cast<uint16_t>(shared_tile[shared_index(local_x + 1, local_y + 1, shared_stride, halo)] ? 1u : 0u) << 8
	);
}

__global__ void tarabek_postprocessing_kernel(
	const cv::cuda::PtrStep<uchar> src,
	cv::cuda::PtrStep<uchar> dst,
	const int num_rows,
	const int num_cols,
	const int halo)
{
	extern __shared__ uchar shared_tile[];

	const int local_x = threadIdx.x;
	const int local_y = threadIdx.y;
	const auto gx = global_index_x(blockIdx.x, local_x, blockDim.x);
	const auto gy = global_index_y(blockIdx.y, local_y, blockDim.y);

	const int shared_stride = blockDim.x + 2 * halo;

	shared_tile[shared_index(local_x, local_y, shared_stride, halo)] = load_center_pixel(
		src, gx, gy, num_cols, num_rows);

	load_halo_edges(shared_tile, src, gx, gy, num_cols, num_rows, shared_stride, local_x, local_y, halo, blockDim);

	load_halo_corners(shared_tile, src, gx, gy, num_cols, num_rows, shared_stride, local_x, local_y, halo, blockDim);

	__syncthreads();

	if (is_out_of_bounds(gx, gy, num_cols, num_rows))
	{
		return;
	}

	if (is_border_pixel(gx, gy, num_cols, num_rows, halo))
	{
		dst(gy, gx) = src(gy, gx);
		return;
	}

	const auto p1 = shared_tile[shared_index(local_x, local_y, shared_stride, halo)];

	if (p1 == background)
	{
		dst(gy, gx) = background;
		return;
	}

	const auto neigh = compute_neigh_bits_shared(shared_tile, shared_stride, local_x, local_y, halo);

	bool to_remove = false;

#pragma unroll
	for (auto index = 0; index < 32; ++index)
	{
		const auto mask_bits = d_templates_a[index];

		if (((neigh ^ mask_bits.fixed_value) & mask_bits.fixed_mask) != 0)
		{
			continue;
		}

		if (!aux_conditions_met_bits_device(mask_bits, neigh))
		{
			continue;
		}

		to_remove = true;
		break;
	}

	if (!to_remove)
	{
#pragma unroll
		for (auto index = 0; index < 16; ++index)
		{
			const auto mask_bits = d_templates_b[index];

			if (((neigh ^ mask_bits.fixed_value) & mask_bits.fixed_mask) != 0)
			{
				continue;
			}

			if (!aux_conditions_met_bits_device(mask_bits, neigh))
			{
				continue;
			}

			to_remove = true;

			break;
		}
	}

	if (to_remove)
	{
		dst(gy, gx) = background;
	}
	else
	{
		dst(gy, gx) = p1;
	}
}

extern inline void tarabek_postprocessing(
	const cv::cuda::GpuMat& src,
	const cv::cuda::GpuMat& dst,
	dim3 grid,
	dim3 block,
	const int halo)
{
	const auto shared_memory = compute_shared_mem_size(block, halo);

	tarabek_postprocessing_kernel<<<grid, block, shared_memory>>>(src, dst, src.rows, src.cols, halo);
}
