module;

#include <opencv2/core.hpp>

export module skeletonizer_gpu:han_la_rhee;

import :core;

namespace skeletonizer::gpu::algorithms
{
	export class han_la_rhee_gpu final : public skeletonizer_gpu,
	                                     public ::skeletonizer::algorithms::han_la_rhee
	{
		void apply(cv::Mat& binary_image) const override
		{
		}
	};
}
