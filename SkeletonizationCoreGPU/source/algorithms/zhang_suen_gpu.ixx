module;

#include <opencv2/core.hpp>

export module skeletonizer_gpu:zhang_suen;

import skeletonizer;
import :core;

export namespace skeletonizer::gpu::algorithms
{
	export class zhang_suen_gpu final : public ::skeletonizer::algorithms::zhang_suen, public skeletonizer_gpu
	{
		void apply(cv::Mat& binary_image) const override
		{
		}
	};
}
