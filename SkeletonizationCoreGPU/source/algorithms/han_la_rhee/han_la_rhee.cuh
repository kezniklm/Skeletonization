#pragma once

#include "../gpu_common.cuh"

#include <cuda_runtime.h>
#include "opencv2/core.hpp"

void han_la_rhee_iteration(
	const cv::cuda::GpuMat& binary_image,
	cv::cuda::PtrStep<uchar> weight,
	int* d_changed, dim3 grid,
	dim3 block, int halo);

void calculate_weight(const cv::cuda::GpuMat& image, cv::cuda::GpuMat& weight,
                      dim3 block, dim3 grid, int halo);
