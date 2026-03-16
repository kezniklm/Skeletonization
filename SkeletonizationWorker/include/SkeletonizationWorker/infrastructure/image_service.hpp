/**
*
* @file image_service.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares the infrastructure image service implementation.
*
* This file defines OpenCV-backed image service implementation for
* loading and saving worker image artifacts.
*
* Main responsibilities:
* - load image files into matrices
* - save image matrices to filesystem
* - implement image service interface contract
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <expected>
#include <string>

#include <opencv2/opencv.hpp>

#include "SkeletonizationWorker/application/interfaces/image_service.hpp"

namespace worker::infrastructure
{
	/**
	 * @class image_service
	 * @brief Implements image loading and saving operations.
	 */
	class image_service final : public application::interfaces::i_image_service
	{
	public:
		/**
		 * @brief Loads image from path.
		 *
		 * @param path Input image path.
		 * @return Loaded image matrix or error message.
		 */
		std::expected<cv::Mat, std::string> load_image(const std::string& path) override;
		/**
		 * @brief Saves image to path.
		 *
		 * @param image Image matrix to save.
		 * @param path Output path.
		 * @return Empty result on success or error message.
		 */
		std::expected<void, std::string> save_image(const cv::Mat& image, const std::string& path) override;
	};
}
