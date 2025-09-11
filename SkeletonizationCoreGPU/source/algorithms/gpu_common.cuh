#pragma once

#include <cuda_runtime.h>
#include "opencv2/core/cuda_types.hpp"

constexpr int DEFAULT_HALO = 1;

__device__ __forceinline__ int shared_index(const int local_x, const int local_y, const int shared_stride,
                                            const int halo)
{
	return (local_y + halo) * shared_stride + (local_x + halo);
}

__device__ __forceinline__ int global_index_x(const int block_x, const int local_x, const int block_dim_x)
{
	return block_x * block_dim_x + local_x;
}

__device__ __forceinline__ int global_index_y(const int block_y, const int local_y, const int block_dim_y)
{
	return block_y * block_dim_y + local_y;
}

__host__ __forceinline__ size_t compute_shared_mem_size(const dim3 block, const int halo = DEFAULT_HALO)
{
	return (block.x + 2 * halo) * (block.y + 2 * halo) * sizeof(uchar);
}

__device__ __forceinline__ bool is_out_of_bounds(const int global_x, const int global_y, const int num_cols,
                                                 const int num_rows)
{
	return global_x >= num_cols || global_y >= num_rows;
}

__device__ __forceinline__ bool is_border_pixel(const int x, const int y, const int num_cols,
                                                const int num_rows)
{
	return x <= 0 || y <= 0 || x == num_cols - 1 || y == num_rows - 1;
}

__device__ __forceinline__ uchar load_center_pixel(
	const cv::cuda::PtrStep<uchar>& src,
	const int global_x, const int global_y,
	const int num_cols, const int num_rows
)
{
	return (global_x >= 0 && global_x < num_cols && global_y >= 0 && global_y < num_rows)
		       ? src(global_y, global_x)
		       : 0;
}

__device__ __forceinline__ void load_halo_edges(
	uchar* shared_tile,
	const cv::cuda::PtrStep<uchar>& src,
	const int global_x, const int global_y,
	const int num_cols, const int num_rows,
	const int shared_stride,
	const int local_x, const int local_y,
	const int halo,
	const dim3 block_dim
)
{
	for (int i = 1; i <= halo; ++i)
	{
		// Left
		if (local_x < halo)
			shared_tile[shared_index(local_x - i, local_y, shared_stride, halo)] =
				load_center_pixel(src, global_x - i, global_y, num_cols, num_rows);
		// Right
		if (local_x >= block_dim.x - halo)
			shared_tile[shared_index(local_x + i, local_y, shared_stride, halo)] =
				load_center_pixel(src, global_x + i, global_y, num_cols, num_rows);
		// Top
		if (local_y < halo)
			shared_tile[shared_index(local_x, local_y - i, shared_stride, halo)] =
				load_center_pixel(src, global_x, global_y - i, num_cols, num_rows);
		// Bottom
		if (local_y >= block_dim.y - halo)
			shared_tile[shared_index(local_x, local_y + i, shared_stride, halo)] =
				load_center_pixel(src, global_x, global_y + i, num_cols, num_rows);
	}
}

__device__ __forceinline__ void load_halo_corners(
	uchar* shared_tile,
	const cv::cuda::PtrStep<uchar>& src,
	const int global_x, const int global_y,
	const int num_cols, const int num_rows,
	const int shared_stride,
	const int local_x, const int local_y,
	const int halo,
	const dim3 block_dim
)
{
	for (int i = 1; i <= halo; ++i)
	{
		for (int j = 1; j <= halo; ++j)
		{
			// Top-left
			if (local_x < halo && local_y < halo)
				shared_tile[shared_index(local_x - i, local_y - j, shared_stride, halo)] =
					load_center_pixel(src, global_x - i, global_y - j, num_cols, num_rows);
			// Top-right
			if (local_x >= block_dim.x - halo && local_y < halo)
				shared_tile[shared_index(local_x + i, local_y - j, shared_stride, halo)] =
					load_center_pixel(src, global_x + i, global_y - j, num_cols, num_rows);
			// Bottom-left
			if (local_x < halo && local_y >= block_dim.y - halo)
				shared_tile[shared_index(local_x - i, local_y + j, shared_stride, halo)] =
					load_center_pixel(src, global_x - i, global_y + j, num_cols, num_rows);
			// Bottom-right
			if (local_x >= block_dim.x - halo && local_y >= block_dim.y - halo)
				shared_tile[shared_index(local_x + i, local_y + j, shared_stride, halo)] =
					load_center_pixel(src, global_x + i, global_y + j, num_cols, num_rows);
		}
	}
}
