#pragma once

#include "gpu_common.cuh"

#include "SkeletonizationCore/skeletonizer/backend.hpp"
#include "SkeletonizationCore/skeletonizer/algorithms.hpp"

namespace skeletonizer::gpu::algorithms
{
	class tarabek final : public ::skeletonizer::algorithms::tarabek, public backend_gpu<14, 14>
	{
	public:
		void apply(cv::Mat& binary_image) const override;
	};
}

void tarabek_iteration(
	const cv::cuda::GpuMat& src,
	const cv::cuda::GpuMat& dst,
	bool first_pass,
	int* d_changed,
	dim3 grid,
	dim3 block,
	int halo);

void tarabek_postprocessing(
	const cv::cuda::GpuMat& src,
	const cv::cuda::GpuMat& dst,
	dim3 grid,
	dim3 block,
	int halo);
