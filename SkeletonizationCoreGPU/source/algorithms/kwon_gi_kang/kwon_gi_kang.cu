#include "../gpu_common.cuh"

#include <cuda_runtime.h>
#include "opencv2/core.hpp"
#include "opencv2/core/cuda.hpp"
#include "opencv2/core/cuda_types.hpp"
#include "opencv2/core/hal/interface.h"

__global__ void kwon_gi_kang_iteration_kernel(
	const cv::cuda::PtrStep<uchar> src,
	cv::cuda::PtrStep<uchar> dst,
	const int num_rows,
	const int num_cols,
	const bool first_pass,
	int* d_changed,
	const int halo)
{
	extern __shared__ uchar shared_tile[];

	const auto local_x = threadIdx.x;
	const auto local_y = threadIdx.y;
	const auto global_x = global_index_x(blockIdx.x, local_x, blockDim.x);
	const auto global_y = global_index_y(blockIdx.y, local_y, blockDim.y);

	const auto shared_stride = blockDim.x + 2 * halo;

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

	if (is_border_pixel(global_x, global_y, num_cols, num_rows))
	{
		dst(global_y, global_x) = src(global_y, global_x);
		return;
	}

	const auto pi = shared_tile[shared_index(local_x, local_y, shared_stride, halo)];

	if (pi == background)
	{
		dst(global_y, global_x) = background;
		return;
	}

	const auto p1 = shared_tile[shared_index(local_x - 1, local_y - 1, shared_stride, halo)];
	const auto p2 = shared_tile[shared_index(local_x, local_y - 1, shared_stride, halo)];
	const auto p3 = shared_tile[shared_index(local_x + 1, local_y - 1, shared_stride, halo)];
	const auto p4 = shared_tile[shared_index(local_x + 1, local_y, shared_stride, halo)];
	const auto p5 = shared_tile[shared_index(local_x + 1, local_y + 1, shared_stride, halo)];
	const auto p6 = shared_tile[shared_index(local_x, local_y + 1, shared_stride, halo)];
	const auto p7 = shared_tile[shared_index(local_x - 1, local_y + 1, shared_stride, halo)];
	const auto p8 = shared_tile[shared_index(local_x - 1, local_y, shared_stride, halo)];

	// A: Transition count (0→1 along neighbors)
	const auto a =
		(p1 == 0 && p2 == 1) +
		(p2 == 0 && p3 == 1) +
		(p3 == 0 && p4 == 1) +
		(p4 == 0 && p5 == 1) +
		(p5 == 0 && p6 == 1) +
		(p6 == 0 && p7 == 1) +
		(p7 == 0 && p8 == 1) +
		(p8 == 0 && p1 == 1);

	// B: Neighbor count (number of foreground pixels)
	const auto b = p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8;

	// C, D: Connectivity checks
	const auto step_condition_c = first_pass ? p2 * p4 * p6 : p2 * p4 * p8;
	const auto step_condition_d = first_pass ? p4 * p6 * p8 : p2 * p6 * p8;

	if (a == 1 && b >= 2 && b <= 6 && step_condition_c == 0 && step_condition_d == 0)
	{
		dst(global_y, global_x) = background;
		atomicExch(d_changed, 1);
	}
	else
	{
		dst(global_y, global_x) = skeleton;
	}
}

__global__ void cleanup_oblique_corners_kernel_opt(
	const cv::cuda::PtrStep<uchar> src,
	cv::cuda::PtrStep<uchar> dst,
	const int num_rows,
	const int num_cols,
	const int halo)
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

	if (is_border_pixel(global_x, global_y, num_cols, num_rows))
	{
		dst(global_y, global_x) = src(global_y, global_x);
		return;
	}

	const auto pi = shared_tile[shared_index(local_x, local_y, shared_stride, halo)];

	if (pi == background)
	{
		dst(global_y, global_x) = background;
		return;
	}

	const auto p1 = shared_tile[shared_index(local_x - 1, local_y - 1, shared_stride, halo)];
	const auto p3 = shared_tile[shared_index(local_x + 1, local_y - 1, shared_stride, halo)];
	const auto p4 = shared_tile[shared_index(local_x + 1, local_y, shared_stride, halo)];
	const auto p5 = shared_tile[shared_index(local_x + 1, local_y + 1, shared_stride, halo)];
	const auto p6 = shared_tile[shared_index(local_x, local_y + 1, shared_stride, halo)];
	const auto p7 = shared_tile[shared_index(local_x - 1, local_y + 1, shared_stride, halo)];
	const auto p8 = shared_tile[shared_index(local_x - 1, local_y, shared_stride, halo)];

	const auto condition_1 = p3 == 0 && p1 * p6 * p8 == 1;
	const auto condition_2 = p1 == 0 && p3 * p4 * p6 == 1;
	const auto condition_3 = p3 == 0 && p5 * p6 * p8 == 1;
	const auto condition_4 = p1 == 0 && p4 * p6 * p7 == 1;

	dst(global_y, global_x) = condition_1 || condition_2 || condition_3 || condition_4 ? background : skeleton;
}

extern inline void kwon_gi_kang_iteration(
	const cv::cuda::GpuMat& src,
	const cv::cuda::GpuMat& dst,
	const bool first_pass,
	int* d_changed,
	const dim3 grid,
	const dim3 block,
	const int halo)
{
	const size_t shared_mem = compute_shared_mem_size(block, halo);

	kwon_gi_kang_iteration_kernel << <grid, block, shared_mem >> >(src, dst, src.rows, src.cols, first_pass,
	                                                               d_changed, halo);
}

extern inline void cleanup_oblique_corners(
	const cv::cuda::GpuMat& src,
	const cv::cuda::GpuMat& dst,
	const dim3 grid,
	const dim3 block,
	const int halo)
{
	const size_t shared_mem = compute_shared_mem_size(block, halo);

	cleanup_oblique_corners_kernel_opt << <grid, block, shared_mem >> >(
		src, dst, src.rows, src.cols, halo);
}
