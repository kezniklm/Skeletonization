/**
*
* @file skeletonization_processor.cpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Implements infrastructure-backed skeletonization processing.
*
* This file implements algorithm lookup and skeletonization execution.
*
* Main responsibilities:
* - resolve algorithms through factory lookup
* - run preprocessing when enabled
* - execute skeletonization and return result image
*
* @version 1.0
* @date 2026-03-16
*/

#include "SkeletonizationWorker/infrastructure/skeletonization_processor.hpp"

#include <glog/logging.h>

#include <opencv2/core.hpp>

#include "SkeletonizationCore/extensions/image_processing.hpp"

namespace worker::infrastructure
{
	skeletonizer::skeletonizer_type resolve_processor_type(const std::string& algorithm_name,
	                                                       const skeletonizer::skeletonizer_type requested_type)
	{
		if (skeletonizer::algorithm_factory::supports(algorithm_name, requested_type))
		{
			return requested_type;
		}

		if (requested_type != skeletonizer::skeletonizer_type::cpu &&
			skeletonizer::algorithm_factory::supports(algorithm_name, skeletonizer::skeletonizer_type::cpu))
		{
			LOG(WARNING)
				<< "Algorithm '" << algorithm_name
				<< "' is not available for processor type '" << skeletonizer::to_string(requested_type)
				<< "'. Falling back to '" << skeletonizer::to_string(skeletonizer::skeletonizer_type::cpu) << "'.";

			return skeletonizer::skeletonizer_type::cpu;
		}

		return requested_type;
	}

	std::expected<cv::Mat, std::string> skeletonization_processor::process(
		const cv::Mat& input_image,
		const std::string& algorithm_name,
		const bool should_preprocess)
	{
		try
		{
			cv::Mat binary_image;

			if (should_preprocess)
			{
				binary_image = image_processor_.preprocess(input_image);
				LOG(INFO) << "Preprocessed image for algorithm: " << algorithm_name;
			}
			else
			{
				const auto normalized = image_processor_.normalize_binary_image(input_image);
				if (!normalized)
				{
					return std::unexpected(normalized.error());
				}
				binary_image = normalized.value();
			}

			const auto resolved_processor_type = resolve_processor_type(algorithm_name, processor_type_);

			const auto skeletonizer = skeletonizer::algorithm_factory::create(algorithm_name, resolved_processor_type);

			if (!skeletonizer)
			{
				return std::unexpected(
					"Unknown algorithm or unsupported processor type: algorithm='" + algorithm_name +
					"', processor_type='" + skeletonizer::to_string(processor_type_) + "'");
			}

			skeletonizer->apply(binary_image);

			LOG(INFO) << "Skeletonization completed: " << algorithm_name << " (" << skeletonizer::to_string(
					resolved_processor_type)
				<< ")";

			return scale(binary_image);
		}
		catch (const std::exception& e)
		{
			return std::unexpected(std::string{"Processing error: "} + e.what());
		}
	}
}
