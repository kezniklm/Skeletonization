module;

#include <opencv2/core.hpp>

export module skeletonizer_gpu:hesselink_roerdink;

import :core;

export namespace skeletonizer::gpu::algorithms
{
	class hesselink_roerdink_gpu final : public skeletonizer_gpu<>,
	                                     public ::skeletonizer::algorithms::hesselink_roerdink
	{
		void apply(cv::Mat& binary_image) const override
		{
		}
	};
}
