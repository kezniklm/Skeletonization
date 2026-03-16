/**
*
* @file image_container.cpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Implements visual inspector image container behavior.
*
* This file implements image container storage and retrieval operations.
*
* Main responsibilities:
* - store labeled benchmark images
* - provide indexed image and label access
* - manage container naming and size reporting
*
* @version 1.0
* @date 2026-03-16
*/

#include "glog/logging.h"

#include "SkeletonizationCLI/visual_inspector/image_container.hpp"

namespace visual_inspector
{
	image_container::image_container(std::string name)
		: name_(std::move(name))
	{
	}

	void image_container::add_image(const cv::Mat& image)
	{
		if (image.empty())
		{
			LOG(WARNING) << "Attempted to add empty image. Skipping image "
				<< name_ << " ...";
			return;
		}

		images_.push_back(image);
		labels_.push_back("");
	}

	void image_container::add_image(const cv::Mat& image, const std::string& label)
	{
		if (image.empty())
		{
			LOG(WARNING) << "Attempted to add empty image. Skipping image "
				<< name_ << " ...";
			return;
		}

		images_.push_back(image);
		labels_.push_back(label);
	}

	std::size_t image_container::size() const noexcept
	{
		return images_.size();
	}

	const cv::Mat& image_container::image(const std::size_t index) const
	{
		return images_.at(index);
	}

	const std::string& image_container::label(const std::size_t index) const
	{
		return labels_.at(index);
	}

	const std::string& image_container::name() const noexcept
	{
		return name_;
	}
}
