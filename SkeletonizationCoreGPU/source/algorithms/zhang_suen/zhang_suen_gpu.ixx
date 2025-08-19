module;

#include "opencv2/core.hpp"
#include "opencv2/core/cuda.hpp"
#include "cuda_runtime.h"
#include "zhang_suen.cuh"

export module skeletonizer_gpu:zhang_suen;

import skeletonizer;
import :core;

export namespace skeletonizer::gpu::algorithms
{
	export class zhang_suen_gpu final : public ::skeletonizer::algorithms::zhang_suen, public skeletonizer_gpu
	{
		void apply(cv::Mat& binary_image) const override
		{
			cv::cuda::GpuMat gpu_src(binary_image);
			cv::cuda::GpuMat gpu_dst(binary_image.size(), gpu_src.type());

			constexpr dim3 block(block_dimension, block_dimension);
			const dim3 grid((gpu_src.cols + block.x - 1) / block.x,
			                (gpu_src.rows + block.y - 1) / block.y);

			int host_changed, *device_changed = nullptr;

			cudaMalloc(&device_changed, sizeof(int));

			cv::cuda::GpuMat *src = &gpu_src, *dst = &gpu_dst;

			do
			{
				cudaMemset(device_changed, 0, sizeof(int));

				zhang_suen_iteration(*src, *dst, true, device_changed, grid, block);

				std::swap(src, dst);

				zhang_suen_iteration(*src, *dst, false, device_changed, grid, block);

				std::swap(src, dst);

				cudaMemcpy(&host_changed, device_changed, sizeof(int), cudaMemcpyDeviceToHost);
			}
			while (host_changed != 0);

			cudaFree(device_changed);

			src->download(binary_image);

			cudaDeviceSynchronize();
		}
	};
}
