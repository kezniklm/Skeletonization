#include "SkeletonizationCoreGPU/liu_zhang.cuh"

namespace skeletonizer::gpu::algorithms
{
	void liu_zhang::apply(cv::Mat& binary_image) const
	{
		cv::cuda::GpuMat gpu_a(binary_image);
		cv::cuda::GpuMat gpu_b(binary_image.size(), gpu_a.type());

		const dim3 block(block_dimension_x, block_dimension_y);
		const dim3 grid(
			(gpu_a.cols + block.x - 1) / block.x,
			(gpu_a.rows + block.y - 1) / block.y);

		int host_changed = 0;
		int* device_changed = nullptr;

		cudaMalloc(&device_changed, sizeof(int));

		cv::cuda::GpuMat* src = &gpu_a;
		cv::cuda::GpuMat* dst = &gpu_b;

		while (true)
		{
			cudaMemset(device_changed, 0, sizeof(int));
			liu_zhang_iteration(
				*src, *dst,
				true,
				true,
				device_changed,
				grid, block, halo);
			std::swap(src, dst);

			cudaMemset(device_changed, 0, sizeof(int));
			liu_zhang_iteration(
				*src, *dst,
				false,
				true,
				device_changed,
				grid, block, halo);
			std::swap(src, dst);

			cudaMemset(device_changed, 0, sizeof(int));
			liu_zhang_iteration(
				*src, *dst,
				true,
				false,
				device_changed,
				grid, block, halo);
			std::swap(src, dst);

			cudaMemcpy(&host_changed,
			           device_changed,
			           sizeof(int),
			           cudaMemcpyDeviceToHost);

			if (host_changed == 0)
			{
				break;
			}

			cudaMemset(device_changed, 0, sizeof(int));
			liu_zhang_iteration(
				*src, *dst,
				false,
				false,
				device_changed,
				grid, block, halo);
			std::swap(src, dst);

			cudaMemcpy(&host_changed,
			           device_changed,
			           sizeof(int),
			           cudaMemcpyDeviceToHost);

			if (host_changed == 0)
			{
				break;
			}
		}

		delete_patterns_ghij_gpu(*src, *dst, grid, block, halo);
		std::swap(src, dst);

		cudaFree(device_changed);

		src->download(binary_image);

		clear_border(binary_image);

		cudaDeviceSynchronize();
	}
}

__global__ void liu_zhang_iteration_kernel(
	const cv::cuda::PtrStep<uchar> src,
	cv::cuda::PtrStep<uchar> dst,
	const int num_rows,
	const int num_cols,
	const bool first_pass,
	const bool use_constraint,
	int* d_changed,
	const int halo)
{
	extern __shared__ uchar shared_tile[];

	const int local_x = threadIdx.x;
	const int local_y = threadIdx.y;
	const int global_x = global_index_x(blockIdx.x, local_x, blockDim.x);
	const int global_y = global_index_y(blockIdx.y, local_y, blockDim.y);

	const int shared_stride = blockDim.x + 2 * halo;

	// Center
	shared_tile[shared_index(local_x, local_y, shared_stride, halo)] =
		load_center_pixel(src, global_x, global_y, num_cols, num_rows);

	// Halo
	load_halo_edges(
		shared_tile, src, global_x, global_y,
		num_cols, num_rows, shared_stride,
		local_x, local_y, halo, blockDim);

	load_halo_corners(
		shared_tile, src, global_x, global_y,
		num_cols, num_rows, shared_stride,
		local_x, local_y, halo, blockDim);

	__syncthreads();

	if (is_out_of_bounds(global_x, global_y, num_cols, num_rows))
		return;

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

	// A: transition count (0->1 around the 8-neighborhood)
	const int a =
		(p2 == 0 && p3 == 1) +
		(p3 == 0 && p4 == 1) +
		(p4 == 0 && p5 == 1) +
		(p5 == 0 && p6 == 1) +
		(p6 == 0 && p7 == 1) +
		(p7 == 0 && p8 == 1) +
		(p8 == 0 && p9 == 1) +
		(p9 == 0 && p2 == 1);

	// B: number of 1-valued neighbors
	const int b = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;

	// Connectivity step conditions
	const int step_condition_c = first_pass ? (p2 * p4 * p6) : (p2 * p4 * p8);
	const int step_condition_d = first_pass ? (p4 * p6 * p8) : (p2 * p6 * p8);

	// Cp constraint (Liu–Zhang)
	bool is_cp_ok = true;
	if (use_constraint)
	{
		const int Cp = p2 + p4 + p6 + p8;
		is_cp_ok = (Cp < 3);
	}

	if (a == 1 && b >= 2 && b <= 6 && step_condition_c == 0 && step_condition_d == 0 && is_cp_ok)
	{
		dst(global_y, global_x) = background;
		atomicExch(d_changed, 1);
	}
	else
	{
		dst(global_y, global_x) = skeleton;
	}
}

__global__ void delete_patterns_ghij_kernel(
	const cv::cuda::PtrStep<uchar> src,
	cv::cuda::PtrStep<uchar> dst,
	const int num_rows,
	const int num_cols,
	const int halo)
{
	const int local_x = threadIdx.x;
	const int local_y = threadIdx.y;
	const int global_x = global_index_x(blockIdx.x, local_x, blockDim.x);
	const int global_y = global_index_y(blockIdx.y, local_y, blockDim.y);

	if (is_out_of_bounds(global_x, global_y, num_cols, num_rows))
		return;

	if (is_border_pixel(global_x, global_y, num_cols, num_rows, halo))
	{
		dst(global_y, global_x) = src(global_y, global_x);
		return;
	}

	const auto p1 = src(global_y, global_x);

	if (p1 == background)
	{
		dst(global_y, global_x) = background;
		return;
	}

	const bool p2 = src(global_y - 1, global_x) > 0;
	const bool p3 = src(global_y - 1, global_x + 1) > 0;
	const bool p4 = src(global_y, global_x + 1) > 0;
	const bool p5 = src(global_y + 1, global_x + 1) > 0;
	const bool p6 = src(global_y + 1, global_x) > 0;
	const bool p7 = src(global_y + 1, global_x - 1) > 0;
	const bool p8 = src(global_y, global_x - 1) > 0;
	const bool p9 = src(global_y - 1, global_x - 1) > 0;

	const int Np =
		static_cast<int>(p2) + static_cast<int>(p3) +
		static_cast<int>(p4) + static_cast<int>(p5) +
		static_cast<int>(p6) + static_cast<int>(p7) +
		static_cast<int>(p8) + static_cast<int>(p9);

	bool delete_pixel = false;

	if (Np == 2)
	{
		const bool step_condition_g = p6 && p8 && !p3;
		const bool step_condition_h = p4 && p6 && !p9;
		const bool step_condition_i = p2 && p4 && !p7;
		const bool step_condition_j = p2 && p8 && !p5;

		delete_pixel = step_condition_g || step_condition_h ||
			step_condition_i || step_condition_j;
	}

	dst(global_y, global_x) = delete_pixel ? background : skeleton;
}

void liu_zhang_iteration(
	const cv::cuda::GpuMat& src,
	const cv::cuda::GpuMat& dst,
	const bool first_pass,
	const bool use_constraint,
	int* d_changed,
	dim3 grid,
	dim3 block,
	const int halo)
{
	const auto shared_mem = compute_shared_mem_size(block, halo);

	liu_zhang_iteration_kernel << <grid, block, shared_mem >> >(
		src, dst,
		src.rows, src.cols,
		first_pass,
		use_constraint,
		d_changed,
		halo);
}

void delete_patterns_ghij_gpu(
	const cv::cuda::GpuMat& src,
	const cv::cuda::GpuMat& dst,
	dim3 grid,
	dim3 block,
	const int halo)
{
	delete_patterns_ghij_kernel << <grid, block >> >(
		src, dst,
		src.rows, src.cols,
		halo);
}
