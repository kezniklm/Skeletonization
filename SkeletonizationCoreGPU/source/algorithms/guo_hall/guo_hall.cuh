#pragma once

#include <cuda_runtime.h>
#include "opencv2/core.hpp"

void guo_hall_iteration(
	const cv::cuda::GpuMat& src,
	const cv::cuda::GpuMat& dst,
	bool first_pass,
	int* d_changed,
	dim3 grid,
	dim3 block);
