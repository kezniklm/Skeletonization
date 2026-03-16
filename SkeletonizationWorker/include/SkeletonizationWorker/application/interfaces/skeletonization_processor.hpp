/**
*
* @file skeletonization_processor.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares the skeletonization processor contract.
 *
 * This file defines interface for running skeletonization algorithms.
 *
 * Main responsibilities:
 * - define skeletonization processing contract
 * - accept algorithm name and preprocessing flag
 * - return processed image or failure reason
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
	 * @class i_skeletonization_processor
	 * @brief Defines skeletonization execution operations.
	 */
	class i_skeletonization_processor
	{
	public:
		/**
		 * @brief Destroys the skeletonization processor interface.
		 */
		virtual ~i_skeletonization_processor() = default;

		/**
		 * @brief Runs skeletonization on an input image.
		 *
		 * @param input_image Input image matrix.
		 * @param algorithm_name Algorithm identifier.
		 * @param should_preprocess Enables preprocessing before skeletonization.
		 * @return Skeletonized image on success or error message.
		 */
		virtual std::expected<cv::Mat, std::string> process(const cv::Mat& input_image,
		                                                    const std::string& algorithm_name,
		                                                    bool should_preprocess) = 0;
	};
}
