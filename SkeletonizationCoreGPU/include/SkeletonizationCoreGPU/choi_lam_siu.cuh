#pragma once

#include "gpu_common.cuh"

#include "SkeletonizationCore/skeletonizer/backend.hpp"
#include "SkeletonizationCore/skeletonizer/algorithms.hpp"

namespace skeletonizer::gpu::algorithms
{
	class choi_lam_siu final : public ::skeletonizer::algorithms::choi_lam_siu, public backend_gpu<32>
	{
	public:
		void apply(cv::Mat& binary_image) const override;
	};
}

extern inline cv::cuda::GpuMat build_label_to_background_point_lut(
	const cv::cuda::GpuMat& binary_image,
	const cv::cuda::GpuMat& label_matrix,
	dim3 block, dim3 grid,
	int lut_size, int halo);

void skeletonize(
	cv::cuda::GpuMat& binary_image,
	const cv::cuda::GpuMat& label_matrix,
	const cv::cuda::GpuMat& lut,
	dim3 block,
	dim3 grid,
	int halo,
	int min_d2 = 100,
	int max_d2 = std::numeric_limits<int>::max());
