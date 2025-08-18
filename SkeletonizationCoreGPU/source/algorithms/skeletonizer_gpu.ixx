module;

#include "opencv2/cudaarithm.hpp"

export module skeletonizer_gpu:core;

import skeletonizer;

namespace skeletonizer::gpu
{
	export class skeletonizer_gpu : virtual public skeletonizer
	{
	};

	cv::cuda::GpuMat operator~(const cv::cuda::GpuMat& mat)
	{
		cv::cuda::GpuMat result;

		cv::cuda::bitwise_not(mat, result);

		return result;
	}
}
