/**
*
* @file opencv_image_loader.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares OpenCV-based image loader implementation.
*
* This file defines OpenCV-backed image loading service implementation.
*
* Main responsibilities:
* - load image matrices from filesystem paths
* - implement image loader interface contract
* - provide default image loading behavior
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include "SkeletonizationCLI/interfaces/i_image_loader.hpp"

namespace cli::utils
{
	/**
	 * @class opencv_image_loader
	 * @brief Default implementation of i_image_loader.
	 *
	 * Uses OpenCV to load images from disk.
	 */
	class opencv_image_loader final : public interfaces::i_image_loader
	{
	public:
		/**
		 * @brief Loads image from filesystem path.
		 *
		 * @param path Image path.
		 * @return Loaded image matrix.
		 */
		[[nodiscard]] cv::Mat load(const std::filesystem::path& path) const override;
	};
}
