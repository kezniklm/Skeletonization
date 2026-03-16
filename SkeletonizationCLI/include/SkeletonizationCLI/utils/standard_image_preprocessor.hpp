/**
*
* @file standard_image_preprocessor.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares standard image preprocessing implementation.
*
* This file defines default image preprocessing service implementation.
*
* Main responsibilities:
* - preprocess image matrices for benchmarking
* - implement image preprocessor interface contract
* - provide default preprocessing pipeline behavior
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include "SkeletonizationCLI/interfaces/i_image_preprocessor.hpp"

namespace cli::utils
{
	/**
	 * @class standard_image_preprocessor
	 * @brief Default implementation of i_image_preprocessor.
	 *
	 * Uses standard image preprocessing operations including
	 * binarization and normalization.
	 */
	class standard_image_preprocessor final : public interfaces::i_image_preprocessor
	{
	public:
		/**
		 * @brief Preprocesses image for benchmark input.
		 *
		 * @param image Input image matrix.
		 * @return Preprocessed image matrix.
		 */
		[[nodiscard]] cv::Mat preprocess(const cv::Mat& image) const override;
	};
}
