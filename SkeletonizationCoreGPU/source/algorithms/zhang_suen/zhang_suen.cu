#include "zhang_suen.cuh"
#include "../gpu_common.cuh"

#include <cuda_runtime.h>
#include "opencv2/core.hpp"
#include "opencv2/core/cuda.hpp"
#include "opencv2/core/cuda_types.hpp"
#include "opencv2/core/hal/interface.h"

__global__ void zhang_suen_iteration_kernel_opt(
	const cv::cuda::PtrStep<uchar> src,
	cv::cuda::PtrStep<uchar> dst,
	const int num_rows,
	const int num_cols,
	const bool first_pass,
	int* d_changed)
{
	extern __shared__ uchar shared_tile[];

	// ---------------------------
	// Thread and block indexing
	// ---------------------------
	const int local_thread_x = threadIdx.x;
	const int local_thread_y = threadIdx.y;
	const int global_pixel_x = blockIdx.x * blockDim.x + local_thread_x;
	const int global_pixel_y = blockIdx.y * blockDim.y + local_thread_y;

	// Shared memory indexing (stride includes halo border)
	const int shared_stride = blockDim.x + 2;
	const int shared_x = local_thread_x + 1; // +1 offset for halo
	const int shared_y = local_thread_y + 1; // +1 offset for halo

	// ---------------------------
	// Load pixels into shared memory
	// ---------------------------
	shared_tile[shared_y * shared_stride + shared_x] = load_center_pixel(
		src, global_pixel_x, global_pixel_y, num_cols, num_rows);
		
	load_halo_edges(shared_tile, src, global_pixel_x, global_pixel_y, num_cols, num_rows, shared_stride, shared_x,
	                shared_y);
	load_halo_corners(shared_tile, src, global_pixel_x, global_pixel_y, num_cols, num_rows, shared_stride);

	__syncthreads();

	// ---------------------------
	// Bounds and border checks
	// ---------------------------
	const bool is_out_of_bounds = global_pixel_x >= num_cols || global_pixel_y >= num_rows;

	if (is_out_of_bounds)
	{
		return;
	}

	const bool is_border_pixel = global_pixel_x == 0 || global_pixel_y == 0 || global_pixel_x == num_cols - 1 ||
		global_pixel_y == num_rows - 1;

	if (is_border_pixel)
	{
		dst(global_pixel_y, global_pixel_x) = src(global_pixel_y, global_pixel_x);
		return;
	}

	const uchar p1 = shared_tile[shared_y * shared_stride + shared_x];
	const uchar p2 = shared_tile[(shared_y - 1) * shared_stride + shared_x];
	const uchar p3 = shared_tile[(shared_y - 1) * shared_stride + shared_x + 1];
	const uchar p4 = shared_tile[shared_y * shared_stride + shared_x + 1];
	const uchar p5 = shared_tile[(shared_y + 1) * shared_stride + shared_x + 1];
	const uchar p6 = shared_tile[(shared_y + 1) * shared_stride + shared_x];
	const uchar p7 = shared_tile[(shared_y + 1) * shared_stride + shared_x - 1];
	const uchar p8 = shared_tile[shared_y * shared_stride + shared_x - 1];
	const uchar p9 = shared_tile[(shared_y - 1) * shared_stride + shared_x - 1];

	if (p1 == 0)
	{
		dst(global_pixel_y, global_pixel_x) = 0;
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
		dst(global_pixel_y, global_pixel_x) = 0;
		atomicExch(d_changed, 1);
	}
	else
	{
		dst(global_pixel_y, global_pixel_x) = 1;
	}
}

extern inline void zhang_suen_iteration(
	const cv::cuda::GpuMat& src,
	const cv::cuda::GpuMat& dst,
	const bool first_pass,
	int* d_changed,
	dim3 grid,
	dim3 block)
{
	size_t shared_mem = (block.x + HALO) * (block.y + HALO) * sizeof(uchar);
	zhang_suen_iteration_kernel_opt << <grid, block, shared_mem >> >(src, dst, src.rows, src.cols, first_pass,
	                                                                 d_changed);
}
