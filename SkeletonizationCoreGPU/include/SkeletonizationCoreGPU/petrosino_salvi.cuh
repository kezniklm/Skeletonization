#pragma once

#include "gpu_common.cuh"

#include "SkeletonizationCore/skeletonizer/backend.hpp"
#include "SkeletonizationCore/skeletonizer/algorithms.hpp"

namespace skeletonizer::gpu::algorithms
{
	class petrosino_salvi final : public ::skeletonizer::algorithms::petrosino_salvi, public backend_gpu<14, 14, 2>
	{
	public:
		void apply(cv::Mat& binary_image) const override;
	};
}

void petrosino_salvi_iteration(
	const cv::cuda::GpuMat& src,
	const cv::cuda::GpuMat& dst,
	bool first_pass,
	int* d_changed,
	dim3 grid,
	dim3 block,
	int halo);
