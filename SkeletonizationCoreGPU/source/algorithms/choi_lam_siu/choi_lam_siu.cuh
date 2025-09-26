#pragma once


#include <vector_types.h>

#include "opencv2/core.hpp"

extern inline cv::cuda::GpuMat build_label_to_background_point_lut(
	const cv::cuda::GpuMat& binary_image,
	const cv::cuda::GpuMat& label_matrix,
	dim3 block,
	dim3 grid,
	int lut_size);

void skeletonize(
	cv::cuda::GpuMat& binary_image,
	const cv::cuda::GpuMat& label_matrix,
	const cv::cuda::GpuMat& lut,
	dim3 block,
	dim3 grid,
	int halo,
	int min_d2 = 100,
	int max_d2 = std::numeric_limits<int>::max());
