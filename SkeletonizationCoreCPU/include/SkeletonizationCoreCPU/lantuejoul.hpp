#pragma once

#include <opencv2/core.hpp>

#include "SkeletonizationCore/skeletonizer/backend.hpp"
#include "SkeletonizationCore/skeletonizer/algorithms.hpp"

namespace skeletonizer::cpu::algorithms
{
	class lantuejoul final : public backend_cpu, public ::skeletonizer::algorithms::lantuejoul
	{
	public:
		void apply(cv::Mat& binary_image) const override;
	};
}
