#include "SkeletonizationCoreGPU/choi_lam_siu.cuh"

namespace skeletonizer::gpu::algorithms
{
	void choi_lam_siu::apply(cv::Mat& binary_image) const
	{
		cv::cuda::GpuMat gpu_binary_image(binary_image);

		constexpr dim3 block(block_dimension_x, block_dimension_y);
		const dim3 grid(
			(binary_image.cols + block.x - 1) / block.x,
			(binary_image.rows + block.y - 1) / block.y);

		const auto label_matrix = compute_nearest_background_labels(binary_image);

		const cv::cuda::GpuMat label_matrix_gpu(label_matrix);

		const auto max_label = get_max_array_value(label_matrix);
		const auto lut_size = max_label + 1;

		const auto lut = build_label_to_background_point_lut(
			gpu_binary_image,
			label_matrix_gpu,
			block,
			grid,
			lut_size,
			halo);

		skeletonize(gpu_binary_image, label_matrix_gpu, lut, block, grid, halo);

		gpu_binary_image.download(binary_image);

		clear_border(binary_image);
	}
}

__device__ __constant__ int8_t dx8[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
__device__ __constant__ int8_t dy8[8] = {-1, -1, -1, 0, 0, 1, 1, 1};

__global__ void build_label_to_background_point_lut_kernel(
	cv::cuda::PtrStepSz<uchar> binary_image_pointer,
	cv::cuda::PtrStepSz<int> label_pointer,
	int2* __restrict__ lut, const int lut_size, const int halo)
{
	const int x = global_index_x(blockIdx.x, threadIdx.x, blockDim.x);
	const int y = global_index_y(blockIdx.y, threadIdx.y, blockDim.y);

	if (is_out_of_bounds(x, y, binary_image_pointer.cols, binary_image_pointer.rows) ||
		is_border_pixel(x, y, binary_image_pointer.cols, binary_image_pointer.rows, halo) ||
		binary_image_pointer(y, x) == foreground)
	{
		return;
	}

	const int id = label_pointer(y, x);

	if (id <= 0 || id >= lut_size)
	{
		return;
	}

	lut[id] = make_int2(x, y);
}

__global__ void skeletonizer_kernel(
	const cv::cuda::PtrStepSz<uchar> binary_image, // has to be copied
	const cv::cuda::PtrStepSz<int> labels, // has to be copied
	const int2* __restrict__ lut, const int lut_size,
	cv::cuda::PtrStepSz<uchar> out,
	const int min_d2, const int max_d2, const int halo)
{
	const int shared_stride = blockDim.x + 2 * halo;
	const int lx = threadIdx.x;
	const int ly = threadIdx.y;
	const int gx = global_index_x(blockIdx.x, lx, blockDim.x);
	const int gy = global_index_y(blockIdx.y, ly, blockDim.y);

	extern __shared__ unsigned char shared_memory[];
	const auto label_tile = reinterpret_cast<int*>(shared_memory);
	const auto binary_image_tile = reinterpret_cast<uchar*>(label_tile + shared_stride * (blockDim.y + 2 * halo));

	// center
	binary_image_tile[shared_index(lx, ly, shared_stride, halo)] =
		load_center_pixel(binary_image, gx, gy, binary_image.cols, binary_image.rows);
	label_tile[shared_index(lx, ly, shared_stride, halo)] =
		load_center_pixel(labels, gx, gy, labels.cols, labels.rows);

	// halos
	load_halo_edges(binary_image_tile, binary_image, gx, gy, binary_image.cols, binary_image.rows,
	                shared_stride, lx, ly, halo, blockDim);
	load_halo_edges(label_tile, labels, gx, gy, labels.cols, labels.rows,
	                shared_stride, lx, ly, halo, blockDim);

	load_halo_corners(binary_image_tile, binary_image, gx, gy, binary_image.cols, binary_image.rows,
	                  shared_stride, lx, ly, halo, blockDim);
	load_halo_corners(label_tile, labels, gx, gy, labels.cols, labels.rows,
	                  shared_stride, lx, ly, halo, blockDim);

	__syncthreads();

	if (is_out_of_bounds(gx, gy, binary_image.cols, binary_image.rows) || is_border_pixel(
		gx, gy, binary_image.cols, binary_image.rows, halo))
	{
		if (!is_out_of_bounds(gx, gy, out.cols, out.rows))
		{
			out(gy, gx) = background;
		}

		return;
	}

	const int sIdx = shared_index(lx, ly, shared_stride, halo);

	if (binary_image_tile[sIdx] == background)
	{
		out(gy, gx) = background;
		return;
	}

	const int lid = label_tile[sIdx];
	if (lid <= 0 || lid >= lut_size)
	{
		out(gy, gx) = background;
		return;
	}

	const int2 q = lut[lid];
	const int qx = q.x - gx;
	const int qy = q.y - gy;
	const int r2_q = qx * qx + qy * qy;

	bool keep = false;

#pragma unroll
	for (int k = 0; k < 8; ++k)
	{
		const int label_id = label_tile[shared_index(lx + dx8[k], ly + dy8[k], shared_stride, halo)];

		if (label_id <= 0 || label_id >= lut_size)
		{
			continue;
		}

		const int2 qi = lut[label_id];

		const int nx = gx + dx8[k];
		const int ny = gy + dy8[k];

		// Qi = DM(Pi) + (Δx,Δy)
		const int qix = (qi.x - nx) + dx8[k];
		const int qiy = (qi.y - ny) + dy8[k];

		const int ddx = qix - qx;
		const int ddy = qiy - qy;
		const int d2 = ddx * ddx + ddy * ddy;

		const int r2_qi = qix * qix + qiy * qiy;
		const int delta_r2 = r2_qi - r2_q;

		int max_dir = std::abs(ddx);
		const int ady = std::abs(ddy);

		if (ady > max_dir)
		{
			max_dir = ady;
		}
		if (max_dir < 1)
		{
			max_dir = 1;
		}

		if (d2 < min_d2 || d2 > max_d2 || delta_r2 > max_dir)
		{
			continue;
		}

		keep = true;

		break;
	}

	out(gy, gx) = keep ? skeleton : background;
}

extern inline cv::cuda::GpuMat build_label_to_background_point_lut(
	const cv::cuda::GpuMat& binary_image,
	const cv::cuda::GpuMat& label_matrix,
	const dim3 block, const dim3 grid,
	const int lut_size, const int halo)
{
	cv::cuda::GpuMat lut(1, lut_size, CV_32SC2, cv::Scalar::all(-1));

	build_label_to_background_point_lut_kernel << <grid, block >> >(
		binary_image, label_matrix, lut.ptr<int2>(), lut_size, halo);

	return lut;
}

extern inline void skeletonize(
	cv::cuda::GpuMat& binary_image,
	const cv::cuda::GpuMat& label_matrix,
	const cv::cuda::GpuMat& lut,
	const dim3 block,
	const dim3 grid,
	const int halo,
	const int min_d2,
	const int max_d2)
{
	const auto lut_size = lut.cols;

	const cv::cuda::GpuMat output(binary_image.size(), binary_image.type(), cv::Scalar::all(0));

	const auto tile_w = block.x + 2 * halo;
	const auto tile_h = block.y + 2 * halo;
	const size_t shared_memory_size = tile_w * tile_h * (sizeof(int) + sizeof(uint8_t));

	skeletonizer_kernel<<<grid, block, shared_memory_size>>>(binary_image, label_matrix,
	                                                         lut.ptr<int2>(), lut_size,
	                                                         output, min_d2, max_d2, halo);

	output.copyTo(binary_image);
}
