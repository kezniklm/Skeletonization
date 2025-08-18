module;

#include <opencv2/core.hpp>

export module skeletonizer_gpu:petrosino_salvi;

import skeletonizer;
import :core;

export namespace skeletonizer::gpu::algorithms
{
	export class petrosino_salvi_gpu final : public ::skeletonizer::algorithms::petrosino_salvi, public skeletonizer_gpu
	{
		void apply(cv::Mat& binary_image) const override
		{
		}
	};
}
