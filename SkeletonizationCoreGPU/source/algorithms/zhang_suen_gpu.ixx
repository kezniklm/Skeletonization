module;

#include <opencv2/core.hpp>

export module skeletonizer_gpu:zhang_suen;

import skeletonizer;
import :core;

export namespace skeletonizer::gpu::algorithms
{
	export class zhang_suen_gpu final : public ::skeletonizer::algorithms::zhang_suen, public skeletonizer_gpu
	{
		cv::Mat apply(const cv::Mat& binary_image) const override
		{
			return binary_image;
		}
	};
}
