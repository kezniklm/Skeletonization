#pragma once

#include <expected>
#include <string>

#include <opencv2/opencv.hpp>

#include "SkeletonizationWorker/application/interfaces/image_service.hpp"

namespace worker::infrastructure
{
	class image_service final : public application::interfaces::i_image_service
	{
	public:
		std::expected<cv::Mat, std::string> load_image(const std::string& path) override;
		std::expected<void, std::string> save_image(const cv::Mat& image, const std::string& path) override;
	};
}
