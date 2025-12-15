#pragma once

#include <opencv2/core.hpp>

#include "normalize.hpp"

namespace skeltest
{
	template <class T>
	struct runner
	{
		cv::Mat operator()(const cv::Mat& in) const
		{
			cv::Mat work = to01(in);

			T algorithm;
			algorithm.apply(work);

			return to01(work);
		}
	};
}
