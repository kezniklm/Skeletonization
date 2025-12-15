#pragma once

#include <opencv2/core.hpp>

#include "SkeletonizationCore/skeletonizer/backend.hpp"
#include "SkeletonizationCore/skeletonizer/algorithms.hpp"

namespace skeletonizer::cpu::algorithms
{
	class han_la_rhee final : public backend_cpu, public ::skeletonizer::algorithms::han_la_rhee
	{
	public:
		void apply(cv::Mat& binary_image) const override;

	private:
		static void iteration(cv::Mat& binary_image, cv::Mat& marker, const cv::Mat& weight);

		static void calculate_weight(cv::Mat& image, cv::Mat& weight);
	};
}
