#pragma once

#include "gpu_common.cuh"

#include "SkeletonizationCore/skeletonizer/backend.hpp"
#include "SkeletonizationCore/skeletonizer/algorithms.hpp"

namespace skeletonizer::gpu::algorithms
{
	class han_la_rhee final : public backend_gpu<>, public ::skeletonizer::algorithms::han_la_rhee
	{
	public:
		void apply(cv::Mat& binary_image) const override;
	};
}

void han_la_rhee_iteration(const cv::cuda::GpuMat& binary_image, cv::cuda::PtrStep<uchar> weight, int* d_changed,
                           dim3 grid, dim3 block, int halo);

void calculate_weight(const cv::cuda::GpuMat& image, cv::cuda::GpuMat& weight, dim3 block, dim3 grid, int halo);
