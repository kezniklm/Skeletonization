/**
*
* @file standard_image_processor.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares standard image processing operations.
*
* This file defines the default image processor that performs
* preprocessing and normalization for skeletonization inputs.
*
* Main responsibilities:
* - run the standard preprocessing pipeline
* - normalize binary input matrices when preprocessing is disabled
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <expected>
#include <string>

#include <opencv2/core.hpp>

/**
 * @class standard_image_processor
 * @brief Provides standard preprocessing and normalization helpers.
 */
class standard_image_processor
{
public:
	/**
	 * @brief Preprocesses an image for skeletonization.
	 *
	 * @param image Input image matrix.
	 * @return Preprocessed image matrix.
	 */
	[[nodiscard]] cv::Mat preprocess(const cv::Mat& image) const;

	/**
	 * @brief Normalizes a binary image when preprocessing is disabled.
	 *
	 * @param input Input image matrix.
	 * @return Normalized binary image or error message.
	 */
	[[nodiscard]] std::expected<cv::Mat, std::string> normalize_binary_image(const cv::Mat& input) const;
};
