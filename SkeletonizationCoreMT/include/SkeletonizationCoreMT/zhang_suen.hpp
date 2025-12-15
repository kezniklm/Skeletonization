#pragma once

#include <opencv2/core.hpp>

#include "SkeletonizationCore/skeletonizer/backend.hpp"
#include "SkeletonizationCore/skeletonizer/algorithms.hpp"

namespace skeletonizer::mt::algorithms
{
	class zhang_suen final : public backend_threads, public ::skeletonizer::algorithms::zhang_suen
	{
	public:
		void apply(cv::Mat& binary_image) const override;

	private:
		static void first_iteration(cv::Mat& binary_image, cv::Mat& marker);
		static void second_iteration(cv::Mat& binary_image, cv::Mat& marker);
	};
}
