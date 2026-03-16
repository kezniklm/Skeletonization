#pragma once

#include "gpu_common.cuh"

#include "SkeletonizationCore/skeletonizer/backend.hpp"
#include "SkeletonizationCore/skeletonizer/algorithms.hpp"

namespace skeletonizer::gpu::algorithms
{
	class kwon_kang final : public ::skeletonizer::algorithms::kwon_kang, public backend_gpu<14, 14>
	{
	public:
		void apply(cv::Mat& binary_image) const override;
	};
}

void kwon_kang_iteration(const cv::cuda::GpuMat& src, const cv::cuda::GpuMat& dst, bool first_pass,
                         int* d_changed, dim3 grid, dim3 block, int halo);

extern inline void cleanup_oblique_corners(const cv::cuda::GpuMat& src, const cv::cuda::GpuMat& dst, dim3 grid,
                                           dim3 block, int halo);
