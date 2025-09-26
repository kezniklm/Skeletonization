module;

#include "opencv2/core.hpp"
#include "opencv2/core/cuda.hpp"
#include "cuda_runtime.h"
#include "han_la_rhee.cuh"

export module skeletonizer_gpu:han_la_rhee;

import :core;

export namespace skeletonizer::gpu::algorithms
{
	class han_la_rhee_gpu final : public skeletonizer_gpu<>,
	                              public ::skeletonizer::algorithms::han_la_rhee
	{
		void apply(cv::Mat& binary_image) const override
		{
			cv::cuda::GpuMat gpu_binary_image(binary_image);
			cv::cuda::GpuMat weight(binary_image.size(), gpu_binary_image.type(), cv::Scalar(0));

			constexpr dim3 block(block_dimension_x, block_dimension_y);
			const dim3 grid((gpu_binary_image.cols + block.x - 1) / block.x,
			                (gpu_binary_image.rows + block.y - 1) / block.y);

			int host_changed, *device_changed = nullptr;

			cudaMalloc(&device_changed, sizeof(int));

			do
			{
				cudaMemset(device_changed, 0, sizeof(int));

				calculate_weight(gpu_binary_image, weight, block, grid);

				han_la_rhee_iteration(gpu_binary_image, weight, device_changed, grid, block);

				cudaMemcpy(&host_changed, device_changed, sizeof(int), cudaMemcpyDeviceToHost);
			}
			while (host_changed != 0);

			cudaFree(device_changed);

			gpu_binary_image.download(binary_image);

			clear_border(binary_image);

			cudaDeviceSynchronize();
		}
	};
}
