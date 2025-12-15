#pragma once

#include <opencv2/core.hpp>

#include "SkeletonizationCore/skeletonizer/backend.hpp"
#include "SkeletonizationCore/skeletonizer/algorithms.hpp"

namespace skeletonizer::cpu::algorithms
{
	class liu_zhang final : public backend_cpu, public ::skeletonizer::algorithms::liu_zhang
	{
	public:
		void apply(cv::Mat& binary_image) const override;

	private:
		static bool first_iteration(cv::Mat& binary_image,
		                            cv::Mat& marker,
		                            bool use_constraint);

		static bool second_iteration(cv::Mat& binary_image,
		                             cv::Mat& marker,
		                             bool use_constraint);

		void delete_patterns_ghij(cv::Mat& binary_image,
		                          cv::Mat& marker) const;
	};
}
