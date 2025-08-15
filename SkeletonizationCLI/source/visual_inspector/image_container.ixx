module;

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include "glog/logging.h"

export module visual_inspector:image_container;

namespace visual_inspector
{
	export class image_container
	{
	public:
		explicit image_container(std::string name) : name_(std::move(name))
		{
		}

		void add_image(const cv::Mat& image)
		{
			if (image.empty())
			{
				LOG(WARNING) << "Attempted to add empty image. Skipping image " << name_ << " ...";

				return;
			}

			images_.push_back(image);
			labels_.push_back("");
		}

		void add_image(const cv::Mat& image, const std::string& label)
		{
			if (image.empty())
			{
				LOG(WARNING) << "Attempted to add empty image. Skipping image " << name_ << " ...";

				return;
			}

			images_.push_back(image);
			labels_.push_back(std::move(label));
		}

		size_t size() const noexcept { return images_.size(); }
		const cv::Mat& image(const size_t index) const { return images_.at(index); }
		const std::string& label(const size_t index) const { return labels_.at(index); }
		const std::string& name() const noexcept { return name_; }

	private:
		std::string name_;
		std::vector<cv::Mat> images_;
		std::vector<std::string> labels_;
	};
}
