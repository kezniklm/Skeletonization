module;

#include "opencv2/core.hpp"
#include "opencv2/core/cuda.hpp"
#include "cuda_runtime.h"
#include "liu_zhang.cuh"

export module skeletonizer_gpu:liu_zhang;

import skeletonizer;
import :core;

export namespace skeletonizer::gpu::algorithms
{
	class liu_zhang_gpu final : public ::skeletonizer::algorithms::liu_zhang, public skeletonizer_gpu<14, 14>
	{
	public:
		void apply(cv::Mat& binary_image) const override
		{
			cv::cuda::GpuMat gpu_a(binary_image);
			cv::cuda::GpuMat gpu_b(binary_image.size(), gpu_a.type());

			constexpr dim3 block(block_dimension_x, block_dimension_y);
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
				// First iteration (with constraint)
				cudaMemset(device_changed, 0, sizeof(int));
				liu_zhang_iteration(
					*src, *dst,
					/*first_pass=*/true,
					/*use_constraint=*/true,
					device_changed,
					grid, block, halo);
				std::swap(src, dst);

				// Second iteration (with constraint)
				cudaMemset(device_changed, 0, sizeof(int));
				liu_zhang_iteration(
					*src, *dst,
					/*first_pass=*/false,
					/*use_constraint=*/true,
					device_changed,
					grid, block, halo);
				std::swap(src, dst);

				// First iteration (without constraint)
				cudaMemset(device_changed, 0, sizeof(int));
				liu_zhang_iteration(
					*src, *dst,
					/*first_pass=*/true,
					/*use_constraint=*/false,
					device_changed,
					grid, block, halo);
				std::swap(src, dst);

				cudaMemcpy(&host_changed, device_changed, sizeof(int),
				           cudaMemcpyDeviceToHost);

				if (host_changed == 0)
				{
					// No deletions in first unconstrained pass => exit loop
					break;
				}

				// Second iteration (without constraint)
				cudaMemset(device_changed, 0, sizeof(int));
				liu_zhang_iteration(
					*src, *dst,
					/*first_pass=*/false,
					/*use_constraint=*/false,
					device_changed,
					grid, block, halo);
				std::swap(src, dst);

				cudaMemcpy(&host_changed, device_changed, sizeof(int),
				           cudaMemcpyDeviceToHost);

				if (host_changed == 0)
				{
					// No deletions in second unconstrained pass => exit loop
					break;
				}
			}

			// Final G/H/I/J pattern deletion, once
			delete_patterns_ghij_gpu(*src, *dst, grid, block, halo);
			std::swap(src, dst);

			cudaFree(device_changed);

			src->download(binary_image);

			clear_border(binary_image);

			cudaDeviceSynchronize();
		}
	};
};
