module;

#include <opencv2/core.hpp>

export module tests_common:runner;

import :normalize;

namespace skeltest
{
	export template <class T>
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
