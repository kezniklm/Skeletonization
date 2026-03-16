/**
*
* @file i_image_preprocessor.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares image preprocessing interface.
*
* This file defines the contract for preprocessing image matrices prior
* to benchmark execution.
*
* Main responsibilities:
* - preprocess input images for benchmark pipelines
* - abstract preprocessing strategy from callers
* - support testable preprocessing substitution
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <opencv2/core.hpp>

namespace cli::interfaces
{
	/**
	 * @class i_image_preprocessor
	 * @brief Interface for preprocessing images.
	 *
	 * Abstracts image preprocessing operations (e.g., binarization,
	 * normalization) to enable dependency injection and testability.
	 */
	class i_image_preprocessor
	{
	public:
		/**
		 * @brief Destroys the image preprocessor interface.
		 */
		virtual ~i_image_preprocessor() = default;

		/**
		 * @brief Preprocess an image.
		 * @param image Input image to preprocess.
		 * @return Preprocessed image.
		 */
		[[nodiscard]] virtual cv::Mat preprocess(const cv::Mat& image) const = 0;

	protected:
		i_image_preprocessor() = default;
		i_image_preprocessor(const i_image_preprocessor&) = default;
		i_image_preprocessor& operator=(const i_image_preprocessor&) = default;
		i_image_preprocessor(i_image_preprocessor&&) = default;
		i_image_preprocessor& operator=(i_image_preprocessor&&) = default;
	};
}
