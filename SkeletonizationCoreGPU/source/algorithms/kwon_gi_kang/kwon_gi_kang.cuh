#pragma once

#include "../gpu_common.cuh"

#include <cuda_runtime.h>
#include "opencv2/core.hpp"

void kwon_gi_kang_iteration(
	const cv::cuda::GpuMat& src,
	const cv::cuda::GpuMat& dst,
	bool first_pass,
	int* d_changed,
	dim3 grid,
	dim3 block,
	int halo = DEFAULT_HALO);


extern inline void cleanup_oblique_corners(
	const cv::cuda::GpuMat& src,
	const cv::cuda::GpuMat& dst,
	dim3 grid,
	dim3 block,
	int halo = DEFAULT_HALO);
