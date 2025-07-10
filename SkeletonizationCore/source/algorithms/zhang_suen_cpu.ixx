module;

#include <opencv2/core.hpp>

export module skeletonizer_cpu:zhang_suen;

import :core;

namespace skeletonizer::cpu::algorithms
{
	export class zhang_suen_cpu final : public skeletonizer_cpu, public ::skeletonizer::algorithms::zhang_suen
	{
		cv::Mat apply(const cv::Mat& binary_image) const override
		{
			return binary_image;
		}
	};
}
