module;

#include <opencv2/core.hpp>

export module skeletonizer_gpu:kwon_gi_kang;

import skeletonizer;
import :core;

export namespace skeletonizer::gpu::algorithms
{
	export class kwon_gi_kang_gpu final : public ::skeletonizer::algorithms::kwon_gi_kang, public skeletonizer_gpu
	{
		void apply(cv::Mat& binary_image) const override
		{
		}
	};
}
