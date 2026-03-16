/**
*
* @file i_image_loader.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares image loader interface.
*
* This file defines the contract for loading image matrices used in
* benchmark execution.
*
* Main responsibilities:
* - load image files into matrices
* - abstract loading strategy from callers
* - support testability through interface substitution
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <filesystem>
#include <opencv2/core.hpp>

namespace cli::interfaces
{
	/**
	 * @class i_image_loader
	 * @brief Interface for loading images from disk.
	 *
	 * Abstracts image loading operations to enable dependency
	 * injection and testability (e.g., mock image sources).
	 */
	class i_image_loader
	{
	public:
		/**
		 * @brief Destroys the image loader interface.
		 */
		virtual ~i_image_loader() = default;

		/**
		 * @brief Load an image from the specified path.
		 * @param path Path to the image file.
		 * @return Loaded image as cv::Mat.
		 * @throws image_loading_exception if image cannot be loaded.
		 */
		[[nodiscard]] virtual cv::Mat load(const std::filesystem::path& path) const = 0;

	protected:
		i_image_loader() = default;
		i_image_loader(const i_image_loader&) = default;
		i_image_loader& operator=(const i_image_loader&) = default;
		i_image_loader(i_image_loader&&) = default;
		i_image_loader& operator=(i_image_loader&&) = default;
	};
}
