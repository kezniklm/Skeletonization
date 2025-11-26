module;

#include "opencv2/core.hpp"
#include "opencv2/core/cuda.hpp"
#include "cuda_runtime.h"
#include "tarabek.cuh"

export module skeletonizer_gpu:tarabek;

import skeletonizer;

export namespace skeletonizer::gpu::algorithms
{
	class tarabek final : public ::skeletonizer::algorithms::tarabek, public backend_gpu<14, 14>
	{
	public:
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

				tarabek_iteration(*src, *dst, true, device_changed, grid, block, halo);

				std::swap(src, dst);

				tarabek_iteration(*src, *dst, false, device_changed, grid, block, halo);

				std::swap(src, dst);

				cudaMemcpy(&host_changed, device_changed, sizeof(int), cudaMemcpyDeviceToHost);
			}
			while (host_changed != 0);

			cudaFree(device_changed);

			tarabek_postprocessing(*src, *dst, grid, block, halo);

			std::swap(src, dst);

			src->download(binary_image);

			clear_border(binary_image);

			cudaDeviceSynchronize();
		}
	};
}
