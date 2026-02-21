#pragma once

#include <filesystem>
#include <opencv2/core.hpp>

namespace cli::interfaces
{
	/**
	 * @brief Interface for loading images from disk.
	 *
	 * Abstracts image loading operations to enable dependency
	 * injection and testability (e.g., mock image sources).
	 */
	class i_image_loader
	{
	public:
		virtual ~i_image_loader() = default;

		/**
		 * @brief Load an image from the specified path.
		 * @param path Path to the image file.
		 * @return Loaded image as cv::Mat.
		 * @throws std::exception if image cannot be loaded.
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
