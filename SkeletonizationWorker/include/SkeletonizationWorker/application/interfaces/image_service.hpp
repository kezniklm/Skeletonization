/**
*
* @file image_service.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares the image service contract for worker processing.
 *
 * This file defines image loading and saving interface for worker flows.
 *
 * Main responsibilities:
 * - define image loading contract
 * - define image saving contract
 * - abstract image IO from application services
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <expected>
#include <string>

namespace worker::application::interfaces
{
	/**
	 * @class i_image_service
	 * @brief Defines image load and save operations for worker pipelines.
	 */
	class i_image_service
	{
	public:
		/**
		 * @brief Destroys the image service interface.
		 */
		virtual ~i_image_service() = default;

		/**
		 * @brief Loads an image from a path.
		 *
		 * @param path Input image path.
		 * @return Loaded image on success or error message.
		 */
		virtual std::expected<cv::Mat, std::string> load_image(const std::string& path) = 0;
		/**
		 * @brief Saves an image to a path.
		 *
		 * @param image Image to save.
		 * @param path Output image path.
		 * @return Empty result on success or error message.
		 */
		virtual std::expected<void, std::string> save_image(const cv::Mat& image, const std::string& path) = 0;
	};
}
