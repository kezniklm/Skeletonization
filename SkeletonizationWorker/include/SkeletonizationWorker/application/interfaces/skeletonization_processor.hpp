#pragma once

#include <expected>
#include <string>

namespace worker::application::interfaces
{
	class i_skeletonization_processor
	{
	public:
		virtual ~i_skeletonization_processor() = default;

		virtual std::expected<cv::Mat, std::string> process(const cv::Mat& input_image,
		                                                    const std::string& algorithm_name,
		                                                    bool should_preprocess) = 0;
	};
}
