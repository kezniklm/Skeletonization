#include "../gpu_common.cuh"

#include <cuda_runtime.h>
#include "opencv2/core.hpp"
#include "opencv2/core/cuda.hpp"
#include "opencv2/core/cuda_types.hpp"
#include "opencv2/core/hal/interface.h"

__global__ void zhang_suen_iteration_kernel(
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

	if (is_border_pixel(global_x, global_y, num_cols, num_rows))
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

	if (p1 == 0)
	{
		dst(global_y, global_x) = 0;
		return;
	}

	// A: Transition count (0→1 along neighbors)
	const int a = (p2 == 0 && p3 == 1) +
		(p3 == 0 && p4 == 1) +
		(p4 == 0 && p5 == 1) +
		(p5 == 0 && p6 == 1) +
		(p6 == 0 && p7 == 1) +
		(p7 == 0 && p8 == 1) +
		(p8 == 0 && p9 == 1) +
		(p9 == 0 && p2 == 1);

	// B: Neighbor count (number of foreground pixels)
	const int b = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;

	// C, D: Connectivity checks
	const int step_condition_c = first_pass ? p2 * p4 * p6 : p2 * p4 * p8;
	const int step_condition_d = first_pass ? p4 * p6 * p8 : p2 * p6 * p8;

	if (a == 1 && b >= 2 && b <= 6 && step_condition_c == 0 && step_condition_d == 0)
	{
		dst(global_y, global_x) = 0;
		atomicExch(d_changed, 1);
	}
	else
	{
		dst(global_y, global_x) = 1;
	}
}

extern inline void zhang_suen_iteration(
	const cv::cuda::GpuMat& src,
	const cv::cuda::GpuMat& dst,
	const bool first_pass,
	int* d_changed,
	dim3 grid,
	dim3 block,
	const int halo)
{
	const size_t shared_mem = compute_shared_mem_size(block, halo);

	zhang_suen_iteration_kernel<<<grid, block, shared_mem>>>(src, dst, src.rows, src.cols, first_pass,
	                                                         d_changed, halo);
}
