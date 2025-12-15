#include "SkeletonizationCoreGPU/han_la_rhee.cuh"

namespace skeletonizer::gpu::algorithms
{
	void han_la_rhee::apply(cv::Mat& binary_image) const
	{
		cv::cuda::GpuMat gpu_binary_image(binary_image);
		cv::cuda::GpuMat weight(binary_image.size(),
		                        gpu_binary_image.type(),
		                        cv::Scalar(0));

		constexpr dim3 block(block_dimension_x, block_dimension_y);
		const dim3 grid(
			(gpu_binary_image.cols + block.x - 1) / block.x,
			(gpu_binary_image.rows + block.y - 1) / block.y);

		int host_changed = 0;
		int* device_changed = nullptr;

		cudaMalloc(&device_changed, sizeof(int));

		do
		{
			cudaMemset(device_changed, 0, sizeof(int));

			calculate_weight(gpu_binary_image, weight, block, grid, halo);

			han_la_rhee_iteration(gpu_binary_image,
			                      weight,
			                      device_changed,
			                      grid,
			                      block,
			                      halo);

			cudaMemcpy(&host_changed,
			           device_changed,
			           sizeof(int),
			           cudaMemcpyDeviceToHost);
		}
		while (host_changed != 0);

		cudaFree(device_changed);

		gpu_binary_image.download(binary_image);

		clear_border(binary_image);

		cudaDeviceSynchronize();
	}
}

template <typename... Args>
__device__ __forceinline__ bool any_greater_or_equal_than(const uchar threshold, Args... values)
{
	const uchar array[sizeof...(values)] = {static_cast<uchar>(values)...};
	for (int i = 0; i < sizeof...(values); ++i)
	{
		if (array[i] >= threshold)
		{
			return true;
		}
	}
	return false;
}

template <typename... Args>
__device__ __forceinline__ bool all_non_zero(Args... values)
{
	const uchar array[sizeof...(values)] = {static_cast<uchar>(values)...};
	for (int i = 0; i < sizeof...(values); ++i)
	{
		if (array[i] == 0)
		{
			return false;
		}
	}
	return true;
}

template <typename... Args>
__device__ __forceinline__ bool any_equal_to(const uchar value, Args... values)
{
	const uchar array[sizeof...(values)] = {static_cast<uchar>(values)...};
	for (int i = 0; i < sizeof...(values); ++i)
	{
		if (array[i] == value)
		{
			return true;
		}
	}
	return false;
}

template <typename... Args>
__device__ __forceinline__ bool has_adjacent_non_zero(Args... values)
{
	const uchar arr[sizeof...(values)] = {static_cast<uchar>(values)...};
	const int n = sizeof...(values);
	for (int i = 0; i < n; ++i)
	{
		if (arr[i] != 0 && arr[(i + 1) % n] != 0)
		{
			return true;
		}
	}
	return false;
}

__device__ __forceinline__ bool has_critical_pairs(
	const uchar x1, const uchar x2, const uchar x3, const uchar x4,
	const uchar x5, const uchar x6, const uchar x7, const uchar x8)
{
	return has_adjacent_non_zero(x1, x2, x3, x4, x5, x6, x7, x8) ||
		(x2 && x4) || (x4 && x6) || (x6 && x8) || (x8 && x2);
}

__global__ void han_la_rhee_iteration_kernel(
	cv::cuda::PtrStep<uchar> binary_image,
	const cv::cuda::PtrStep<uchar> weight,
	const int num_rows, const int num_cols,
	int* d_changed, const int halo)
{
	const auto row = blockIdx.y * blockDim.y + threadIdx.y;
	const auto column = blockIdx.x * blockDim.x + threadIdx.x;

	if (is_border_pixel(row, column, num_rows, num_cols, halo))
	{
		return;
	}

	const auto previous_weight = weight.ptr(row - 1);
	const auto current_weight = weight.ptr(row);
	const auto next_weight = weight.ptr(row + 1);

	const auto xi = binary_image.ptr(row)[column];

	if (xi == background || current_weight[column] <= 0 || current_weight[column] >= 8)
	{
		return;
	}

	const uchar x1 = previous_weight[column - 1];
	const uchar x2 = previous_weight[column];
	const uchar x3 = previous_weight[column + 1];
	const uchar x4 = current_weight[column + 1];
	const uchar x5 = next_weight[column + 1];
	const uchar x6 = next_weight[column];
	const uchar x7 = next_weight[column - 1];
	const uchar x8 = current_weight[column - 1];

	const bool should_delete =
		(current_weight[column] == 1 && any_greater_or_equal_than(3, x1, x2, x3, x4, x5, x6, x7, x8)) ||
		(current_weight[column] == 2 && any_greater_or_equal_than(3, x1, x2, x3, x4, x5, x6, x7, x8) &&
			has_critical_pairs(x1, x2, x3, x4, x5, x6, x7, x8)) ||
		(current_weight[column] == 3 && any_greater_or_equal_than(7, x1, x2, x3, x4, x5, x6, x7, x8) &&
			(all_non_zero(x6, x7, x8) || all_non_zero(x1, x2, x3) ||
				all_non_zero(x1, x7, x8) || all_non_zero(x6, x7, x5) ||
				all_non_zero(x3, x4, x5) || all_non_zero(x2, x3, x4) ||
				all_non_zero(x4, x5, x6) || all_non_zero(x8, x1, x2) ||
				all_non_zero(x6, x7, x4) || all_non_zero(x6, x1, x8) ||
				all_non_zero(x6, x3, x4) || all_non_zero(x6, x5, x8))) ||
		(current_weight[column] == 4 && (all_non_zero(x1, x2, x3, x4) || all_non_zero(x1, x2, x7, x8) ||
			all_non_zero(x1, x2, x3, x8) || all_non_zero(x1, x6, x7, x8) ||
			all_non_zero(x5, x6, x7, x8) || all_non_zero(x4, x5, x6, x7) ||
			all_non_zero(x3, x4, x5, x6) || all_non_zero(x5, x2, x3, x4) ||
			all_non_zero(x6, x7, x3, x4) || all_non_zero(x1, x8, x5, x6))) ||
		(current_weight[column] == 5 && any_equal_to(8, x1, x2, x3, x4, x5, x6, x7, x8) &&
			(all_non_zero(x7, x8, x1, x2, x3) || all_non_zero(x7, x8, x1, x5, x6) ||
				all_non_zero(x3, x4, x5, x6, x7) || all_non_zero(x1, x2, x3, x4, x5) ||
				all_non_zero(x4, x5, x6, x7, x8) || all_non_zero(x6, x7, x8, x1, x2) ||
				all_non_zero(x1, x2, x3, x4, x8) || all_non_zero(x2, x3, x4, x5, x6))) ||
		(current_weight[column] == 6 && any_equal_to(8, x1, x2, x3, x4, x5, x6, x7, x8) &&
			(all_non_zero(x3, x4, x5, x6, x7, x8) || all_non_zero(x1, x2, x3, x6, x7, x8) ||
				all_non_zero(x1, x2, x5, x6, x7, x8) || all_non_zero(x1, x4, x5, x6, x7, x8) ||
				all_non_zero(x1, x2, x3, x4, x7, x8) || all_non_zero(x3, x4, x5, x6, x7, x2) ||
				all_non_zero(x3, x4, x5, x6, x1, x2) || all_non_zero(x1, x2, x3, x4, x5, x8))) ||
		(current_weight[column] == 7 && any_equal_to(8, x1, x2, x3, x4, x5, x6, x7, x8) &&
			(all_non_zero(x1, x2, x3, x5, x6, x7, x8) || all_non_zero(x1, x3, x4, x5, x6, x7, x8) ||
				all_non_zero(x1, x2, x3, x4, x5, x6, x7) || all_non_zero(x1, x2, x3, x4, x5, x7, x8)));

	if (should_delete)
	{
		binary_image(row, column) = background;
		atomicExch(d_changed, 1);
	}
}

__global__ void calculate_weight_kernel(const cv::cuda::PtrStep<uchar> image, cv::cuda::PtrStep<uchar> weight,
                                        const int num_rows, const int num_cols, const int halo)
{
	const auto row = blockIdx.y * blockDim.y + threadIdx.y;
	const auto column = blockIdx.x * blockDim.x + threadIdx.x;

	if (is_border_pixel(row, column, num_rows, num_cols, halo))
	{
		return;
	}

	const auto previous = image.ptr(row - 1);
	const auto current = image.ptr(row);
	const auto next = image.ptr(row + 1);

	const auto xi = current[column];

	if (xi == background)
	{
		return;
	}

	const auto x1 = previous[column - 1];
	const auto x2 = previous[column];
	const auto x3 = previous[column + 1];
	const auto x4 = current[column - 1];
	const auto x5 = current[column + 1];
	const auto x6 = next[column - 1];
	const auto x7 = next[column];
	const auto x8 = next[column + 1];

	weight.ptr(row)[column] = x1 + x2 + x3 + x4 + x5 + x6 + x7 + x8;
}

extern inline void han_la_rhee_iteration(
	const cv::cuda::GpuMat& binary_image,
	const cv::cuda::PtrStep<uchar> weight,
	int* d_changed, const dim3 grid,
	const dim3 block, const int halo)
{
	han_la_rhee_iteration_kernel<<<grid, block>>>(binary_image, weight, binary_image.rows, binary_image.cols,
	                                              d_changed, halo);
}

extern inline void calculate_weight(const cv::cuda::GpuMat& image, cv::cuda::GpuMat& weight, const dim3 block,
                                    const dim3 grid, const int halo)
{
	weight.setTo(cv::Scalar(0));

	calculate_weight_kernel<<<grid, block>>>(image, weight, image.rows, image.cols, halo);
}
