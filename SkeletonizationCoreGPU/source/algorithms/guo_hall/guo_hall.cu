#include "../gpu_common.cuh"

#include <cuda_runtime.h>
#include "opencv2/core.hpp"
#include "opencv2/core/cuda.hpp"
#include "opencv2/core/cuda_types.hpp"
#include "opencv2/core/hal/interface.h"

__global__ void guo_hall_iteration_kernel(
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

	const auto p1 = shared_tile[shared_index(local_x, local_y, shared_stride, halo)];

	if (p1 != 1)
	{
		dst(global_y, global_x) = 0;
		return;
	}

	const auto p2 = shared_tile[shared_index(local_x, local_y - 1, shared_stride, halo)];
	const auto p3 = shared_tile[shared_index(local_x + 1, local_y - 1, shared_stride, halo)];
	const auto p4 = shared_tile[shared_index(local_x + 1, local_y, shared_stride, halo)];
	const auto p5 = shared_tile[shared_index(local_x + 1, local_y + 1, shared_stride, halo)];
	const auto p6 = shared_tile[shared_index(local_x, local_y + 1, shared_stride, halo)];
	const auto p7 = shared_tile[shared_index(local_x - 1, local_y + 1, shared_stride, halo)];
	const auto p8 = shared_tile[shared_index(local_x - 1, local_y, shared_stride, halo)];
	const auto p9 = shared_tile[shared_index(local_x - 1, local_y - 1, shared_stride, halo)];

	// ---------------------------
	// Count transitions from 0 → 1 in the 8-neighbor sequence
	// This is used to determine if a pixel is an edge pixel
	// c == 1 means exactly one foreground region is connected around p1
	// ---------------------------
	const auto c = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) +
		(p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) +
		(p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
		(p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);

	// ---------------------------
	// Neighbor counts for Guo-Hall conditions
	// n1 and n2 are sums of ORed neighbor pairs
	// These count the number of non-zero (foreground) pixels in specific groups
	// ---------------------------
	const auto n1 = (p9 | p2) + (p3 | p4) + (p5 | p6) + (p7 | p8);
	const auto n2 = (p2 | p3) + (p4 | p5) + (p6 | p7) + (p8 | p9);

	const auto n = min(n1, n2);

	// ---------------------------
	// Guo-Hall deletion condition 'm'
	// Determines if deletion of p1 would break connectivity
	// Different for first and second sub-iterations
	const auto m = first_pass ? (p6 | p7 | !p9) & p8 : (p2 | p3 | !p5) & p4;

	// ---------------------------
	// Decide whether to delete the current pixel
	// Conditions for deletion:
	// 1. Exactly one 0->1 transition (c == 1)
	// 2. Neighbor count between 2 and 3
	// 3. Connectivity condition not violated (m == 0)
	// ---------------------------
	if (c == 1 && (n >= 2 && n <= 3) && m == 0)
	{
		dst(global_y, global_x) = 0;
		atomicExch(d_changed, 1);
	}
	else
	{
		dst(global_y, global_x) = 1;
	}
}

extern inline void guo_hall_iteration(
	const cv::cuda::GpuMat& src,
	const cv::cuda::GpuMat& dst,
	const bool first_pass,
	int* d_changed,
	const dim3 grid,
	const dim3 block,
	const int halo)
{
	const size_t shared_mem = compute_shared_mem_size(block, halo);

	guo_hall_iteration_kernel << <grid, block, shared_mem >> >(src, dst, src.rows, src.cols, first_pass,
	                                                           d_changed, halo);
}
