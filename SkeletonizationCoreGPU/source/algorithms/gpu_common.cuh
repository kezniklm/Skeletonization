#pragma once

#include <cuda_runtime.h>
#include "opencv2/core/cuda_types.hpp"

constexpr auto NEIGHBOUR_PIXEL = 1;
constexpr auto BORDER = 1;
constexpr auto HALO = NEIGHBOUR_PIXEL + BORDER;

__device__ __forceinline__ inline uchar load_center_pixel(
	const cv::cuda::PtrStep<uchar>& src,
	const int global_pixel_x,
	const int global_pixel_y,
	const int num_cols,
	const int num_rows)
{
	return global_pixel_x < num_cols && global_pixel_y < num_rows
		? src(global_pixel_y, global_pixel_x)
		: 0;
}

__device__ __forceinline__ inline void load_halo_edges(
	uchar* shared_tile,
	const cv::cuda::PtrStep<uchar>& src,
	const int global_pixel_x,
	const int global_pixel_y,
	const int num_cols,
	const int num_rows,
	const int shared_stride,
	const int shared_x,
	const int shared_y)
{
	if (threadIdx.x == 0)
	{
		shared_tile[shared_y * shared_stride] = global_pixel_x > 0 && global_pixel_y < num_rows
			? src(global_pixel_y, global_pixel_x - 1)
			: 0;
	}

	if (threadIdx.x == blockDim.x - 1)
	{
		shared_tile[shared_y * shared_stride + blockDim.x + 1] = global_pixel_x + 1 < num_cols && global_pixel_y <
			num_rows
			? src(global_pixel_y, global_pixel_x + 1)
			: 0;
	}


	if (threadIdx.y == 0)
	{
		shared_tile[shared_x] = global_pixel_y > 0 && global_pixel_x < num_cols
			? src(global_pixel_y - 1, global_pixel_x)
			: 0;
	}


	if (threadIdx.y == blockDim.y - 1)
	{
		shared_tile[(blockDim.y + 1) * shared_stride + shared_x] = global_pixel_y + 1 < num_rows && global_pixel_x <
			num_cols
			? src(global_pixel_y + 1, global_pixel_x)
			: 0;
	}
}

__device__ inline void load_halo_corners(
	uchar* shared_tile,
	const cv::cuda::PtrStep<uchar>& src,
	const int global_pixel_x,
	const int global_pixel_y,
	const int num_cols,
	const int num_rows,
	const int shared_stride)
{
	if (threadIdx.x == 0 && threadIdx.y == 0)
	{
		shared_tile[0] = global_pixel_x > 0 && global_pixel_y > 0
			? src(global_pixel_y - 1, global_pixel_x - 1)
			: 0;
	}


	if (threadIdx.x == blockDim.x - 1 && threadIdx.y == 0)
	{
		shared_tile[blockDim.x + 1] = global_pixel_x + 1 < num_cols && global_pixel_y > 0
			? src(global_pixel_y - 1, global_pixel_x + 1)
			: 0;
	}


	if (threadIdx.x == 0 && threadIdx.y == blockDim.y - 1)
	{
		shared_tile[(blockDim.y + 1) * shared_stride] = global_pixel_x > 0 && global_pixel_y + 1 < num_rows
			? src(global_pixel_y + 1, global_pixel_x - 1)
			: 0;
	}

	if (threadIdx.x == blockDim.x - 1 && threadIdx.y == blockDim.y - 1)
	{
		shared_tile[(blockDim.y + 1) * shared_stride + blockDim.x + 1] = global_pixel_x + 1 < num_cols &&
			global_pixel_y + 1 < num_rows
			? src(global_pixel_y + 1,
				global_pixel_x + 1)
			: 0;
	}
}