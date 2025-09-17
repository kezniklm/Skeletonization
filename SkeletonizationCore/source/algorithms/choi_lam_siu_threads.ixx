module;

#include <opencv2/core.hpp>

export module skeletonizer_cpu:choi_lam_siu_threads;

import :core;
import image_processing;

namespace skeletonizer::cpu::algorithms
{
	export class choi_lam_siu_threads final : public skeletonizer_cpu,
	                                                    public ::skeletonizer::algorithms::choi_lam_siu
	{
	public:
		void apply(cv::Mat& binary_image) const override
		{
		}
	};
}
