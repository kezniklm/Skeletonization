#pragma once

#include <expected>
#include <string>

#include "SkeletonizationCore/skeletonizer/algorithm_factory.hpp"

#include "SkeletonizationWorker/application/interfaces/skeletonization_processor.hpp"

namespace worker::infrastructure
{
	class skeletonization_processor final : public application::interfaces::i_skeletonization_processor
	{
	public:
		explicit skeletonization_processor(
			const skeletonizer::skeletonizer_type processor_type = skeletonizer::skeletonizer_type::cpu)
			: processor_type_(processor_type)
		{
		}

		std::expected<cv::Mat, std::string> process(
			const cv::Mat& input_image,
			const std::string& algorithm_name,
			bool should_preprocess) override;

	private:
		skeletonizer::skeletonizer_type processor_type_;
	};
}
