#pragma once

#include <string>
#include <vector>

#include <opencv2/opencv.hpp>

namespace visual_inspector
{
	class image_container
	{
	public:
		explicit image_container(std::string name);

		void add_image(const cv::Mat& image);
		void add_image(const cv::Mat& image, const std::string& label);

		std::size_t size() const noexcept;

		const cv::Mat& image(std::size_t index) const;
		const std::string& label(std::size_t index) const;
		const std::string& name() const noexcept;

	private:
		std::string name_;
		std::vector<cv::Mat> images_;
		std::vector<std::string> labels_;
	};
}
