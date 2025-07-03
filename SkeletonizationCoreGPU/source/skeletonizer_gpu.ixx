module;

#include "opencv2/core/mat.hpp"

export module skeletonizer_gpu;

import skeletonizer;

export class skeletonizer_gpu : public skeletonizer
{
	cv::Mat apply(const cv::Mat &input) const override
	{
		return input;
	}

	std::string name() const override
	{
		return "gpu";
	}
};
