#pragma once

#include "gpu_common.cuh"

#include "SkeletonizationCore/skeletonizer/backend.hpp"
#include "SkeletonizationCore/skeletonizer/algorithms.hpp"

namespace skeletonizer::gpu::algorithms
{
	class liu_zhang final : public ::skeletonizer::algorithms::liu_zhang, public backend_gpu<14, 14>
	{
	public:
		void apply(cv::Mat& binary_image) const override;
	};
}

void liu_zhang_iteration(
	const cv::cuda::GpuMat& src,
	const cv::cuda::GpuMat& dst,
	bool first_pass,
	bool use_constraint,
	int* d_changed,
	dim3 grid,
	dim3 block,
	int halo);

void delete_patterns_ghij_gpu(
	const cv::cuda::GpuMat& src,
	const cv::cuda::GpuMat& dst,
	dim3 grid,
	dim3 block,
	int halo);
