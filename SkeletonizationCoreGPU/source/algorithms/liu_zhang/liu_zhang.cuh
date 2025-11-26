#pragma once

#include "opencv2/core.hpp"

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
