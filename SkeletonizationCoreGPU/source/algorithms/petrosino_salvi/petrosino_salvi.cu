#include "../gpu_common.cuh"

#include "cuda_runtime.h"

#include "opencv2/core.hpp"
#include "opencv2/core/cuda.hpp"
#include "opencv2/core/cuda_types.hpp"
#include "opencv2/core/hal/interface.h"

__device__ __forceinline__ bool petrosino_salvi_first_pass(const uchar x1, const uchar x2, const uchar x3,
                                                           const uchar x4,
                                                           const uchar x5, const uchar x6, const uchar x7,
                                                           const uchar x8,
                                                           const uchar y2, const uchar y5)
{
	const int a = ((x2 ^ x3) != 0) + ((x3 ^ x4) != 0) + ((x4 ^ x5) != 0) +
		((x5 ^ x6) != 0) + ((x6 ^ x7) != 0) + ((x7 ^ x8) != 0) +
		((x8 ^ x1) != 0) + ((x1 ^ x2) != 0);

	const int b = (x2 != 0) + (x3 != 0) + (x4 != 0) + (x5 != 0) +
		(x6 != 0) + (x7 != 0) + (x8 != 0) + (x1 != 0);

	const int r = x1 && x7 && x8 &&
		((!y5 && x2 && x3 && !x5) || (!y2 && !x3 && x5 && x6));

	return a == 2 && b >= 2 && b <= 6 && r == 0;
}

__device__ __forceinline__ bool petrosino_salvi_second_pass(const uchar x1, const uchar x2, const uchar x3,
                                                            const uchar x4,
                                                            const uchar x5, const uchar x6, const uchar x7,
                                                            const uchar x8)
{
	const int s0 = (x3 && x7) || (x5 && x1);

	const int s1 = (x1 && !x6 && (!x4 || x3)) || (x3 && !x8 && (!x6 || x5)) ||
		(x7 && !x4 && (!x2 || x1)) || (x5 && !x2 && (!x8 || x7));

	const int b = (x2 != 0) + (x3 != 0) + (x4 != 0) + (x5 != 0) +
		(x6 != 0) + (x7 != 0) + (x8 != 0) + (x1 != 0);

	const int r = (x3 && ((x1 && !x8) || (x5 && !x6))) ||
		(x7 && ((!x5 && !x8) || (!x1 && !x6)));

	return !s0 && s1 && r == 0 && b >= 3;
}


__global__ void petrosino_salvi_iteration_kernel(
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

	if (p1 == 0)
	{
		dst(global_y, global_x) = 0;
		return;
	}

	const auto x1 = shared_tile[shared_index(local_x + 1, local_y, shared_stride, halo)];
	const auto x2 = shared_tile[shared_index(local_x + 1, local_y - 1, shared_stride, halo)];
	const auto x3 = shared_tile[shared_index(local_x, local_y - 1, shared_stride, halo)];
	const auto x4 = shared_tile[shared_index(local_x - 1, local_y - 1, shared_stride, halo)];
	const auto x5 = shared_tile[shared_index(local_x - 1, local_y, shared_stride, halo)];
	const auto x6 = shared_tile[shared_index(local_x - 1, local_y + 1, shared_stride, halo)];
	const auto x7 = shared_tile[shared_index(local_x, local_y + 1, shared_stride, halo)];
	const auto x8 = shared_tile[shared_index(local_x + 1, local_y + 1, shared_stride, halo)];

	const auto y2 = shared_tile[shared_index(local_x, local_y + 2, shared_stride, halo)];
	const auto y5 = shared_tile[shared_index(local_x + 2, local_y, shared_stride, halo)];

	if (p1 == 0)
	{
		dst(global_y, global_x) = 0;
		return;
	}

	const bool delete_pixel = first_pass
		                          ? petrosino_salvi_first_pass(x1, x2, x3, x4, x5, x6, x7, x8, y2, y5)
		                          : petrosino_salvi_second_pass(x1, x2, x3, x4, x5, x6, x7, x8);

	if (!delete_pixel)
	{
		dst(global_y, global_x) = 1;
		return;
	}

	dst(global_y, global_x) = 0;
	atomicExch(d_changed, 1);
}

extern inline void petrosino_salvi_iteration(
	const cv::cuda::GpuMat& src,
	const cv::cuda::GpuMat& dst,
	const bool first_pass,
	int* d_changed,
	dim3 grid,
	dim3 block,
	const int halo)
{
	const size_t shared_mem = compute_shared_mem_size(block, halo);

	petrosino_salvi_iteration_kernel << <grid, block, shared_mem >> >(src, dst, src.rows, src.cols, first_pass,
	                                                                  d_changed, halo);
}
