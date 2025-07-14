module;

#include <opencv2/core.hpp>

export module skeletonizer_gpu:guo_hall;

import skeletonizer;
import :core;

export namespace skeletonizer::gpu::algorithms
{
	export class guo_hall_gpu final : public ::skeletonizer::algorithms::guo_hall, public skeletonizer_gpu
	{
		void apply(cv::Mat& binary_image) const override
		{
		}
	};
}
