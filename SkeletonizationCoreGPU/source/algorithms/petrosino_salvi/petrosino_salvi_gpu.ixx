module;

#include "opencv2/core.hpp"
#include "opencv2/core/cuda.hpp"
#include "cuda_runtime.h"
#include "petrosino_salvi.cuh"

export module skeletonizer_gpu:petrosino_salvi;

import skeletonizer;
import :core;

export namespace skeletonizer::gpu::algorithms
{
	class petrosino_salvi_gpu final : public ::skeletonizer::algorithms::petrosino_salvi,
	                                  public skeletonizer_gpu<14, 14, 2>
	{
		void apply(cv::Mat& binary_image) const override
		{
			cv::cuda::GpuMat gpu_src(binary_image);
			cv::cuda::GpuMat gpu_dst(binary_image.size(), gpu_src.type());

			constexpr dim3 block(block_dimension_x, block_dimension_y);
			const dim3 grid((gpu_src.cols + block.x - 1) / block.x,
			                (gpu_src.rows + block.y - 1) / block.y);

			int host_changed, *device_changed = nullptr;

			cudaMalloc(&device_changed, sizeof(int));

			cv::cuda::GpuMat *src = &gpu_src, *dst = &gpu_dst;

			do
			{
				cudaMemset(device_changed, 0, sizeof(int));

				petrosino_salvi_iteration(*src, *dst, true, device_changed, grid, block, halo);

				std::swap(src, dst);

				petrosino_salvi_iteration(*src, *dst, false, device_changed, grid, block, halo);

				std::swap(src, dst);

				cudaMemcpy(&host_changed, device_changed, sizeof(int), cudaMemcpyDeviceToHost);
			}
			while (host_changed != 0);

			cudaFree(device_changed);

			src->download(binary_image);

			clear_border(binary_image);

			cudaDeviceSynchronize();
		}
	};
}
