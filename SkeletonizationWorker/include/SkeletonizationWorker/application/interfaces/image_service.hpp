#pragma once

#include <expected>
#include <string>

namespace worker::application::interfaces
{
	class i_image_service
	{
	public:
		virtual ~i_image_service() = default;

		virtual std::expected<cv::Mat, std::string> load_image(const std::string& path) = 0;
		virtual std::expected<void, std::string> save_image(const cv::Mat& image, const std::string& path) = 0;
	};
}
