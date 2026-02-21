#pragma once

#include "SkeletonizationCLI/interfaces/i_image_loader.hpp"

namespace cli::utils
{
	/**
	 * @brief Default implementation of i_image_loader.
	 *
	 * Uses OpenCV to load images from disk.
	 */
	class opencv_image_loader final : public interfaces::i_image_loader
	{
	public:
		[[nodiscard]] cv::Mat load(const std::filesystem::path& path) const override;
	};
}
