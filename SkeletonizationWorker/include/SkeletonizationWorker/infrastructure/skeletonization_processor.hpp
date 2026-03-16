/**
*
* @file skeletonization_processor.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares infrastructure skeletonization processor implementation.
*
* This file defines skeletonization processor implementation that selects
* algorithms through the algorithm factory.
*
* Main responsibilities:
* - resolve algorithms by name and backend
* - execute skeletonization processing
* - optionally run preprocessing before processing
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <expected>
#include <string>

#include "SkeletonizationCore/skeletonizer/algorithm_factory.hpp"

#include "SkeletonizationWorker/application/interfaces/skeletonization_processor.hpp"

namespace worker::infrastructure
{
	/**
	 * @class skeletonization_processor
	 * @brief Implements algorithm selection and skeletonization execution.
	 */
	class skeletonization_processor final : public application::interfaces::i_skeletonization_processor
	{
	public:
		explicit skeletonization_processor(
			const skeletonizer::skeletonizer_type processor_type = skeletonizer::skeletonizer_type::cpu)
			: processor_type_(processor_type)
		{
		}

		std::expected<cv::Mat, std::string> process(
			const cv::Mat& input_image,
			const std::string& algorithm_name,
			bool should_preprocess) override;

	private:
		/// Configured processor backend type.
		skeletonizer::skeletonizer_type processor_type_;
	};
}
