module;

#include "opencv2/core/mat.hpp"

export module skeletonizer_cpu;

import skeletonizer;

export class skeletonizer_cpu : public skeletonizer
{
	cv::Mat apply(const cv::Mat &input) const override
	{
		return input;
	}

	std::string name() const override
	{
		return "cpu";
	}
};
