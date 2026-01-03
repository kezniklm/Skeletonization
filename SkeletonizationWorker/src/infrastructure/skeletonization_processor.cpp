#include "SkeletonizationWorker/infrastructure/skeletonization_processor.hpp"

#include <glog/logging.h>

#include <opencv2/core.hpp>

#include "SkeletonizationCore/extensions/image_processing.hpp"

namespace worker::infrastructure
{
	static std::expected<cv::Mat, std::string> normalize_binary_image(const cv::Mat& input)
	{
		if (input.empty())
		{
			return std::unexpected("Input image is empty");
		}

		if (input.channels() != 1)
		{
			return std::unexpected(
				"Input image must be single-channel binary (0/1). Enable preprocessing for non-binary images.");
		}

		double min_val = 0.0;
		double max_val = 0.0;

		cv::minMaxLoc(input, &min_val, &max_val);

		if (min_val >= 0.0 && max_val <= 1.0)
		{
			return input;
		}

		if ((min_val != 0.0 || max_val != 255.0) && (min_val != 255.0 || max_val != 255.0))
		{
			return std::unexpected(
				"Input image is not binary (expected 0/1). Enable preprocessing to convert the image before running algorithms.");
		}

		cv::Mat is_zero;
		cv::Mat is_255;
		cv::compare(input, 0, is_zero, cv::CMP_EQ);
		cv::compare(input, 255, is_255, cv::CMP_EQ);

		cv::Mat allowed;
		cv::bitwise_or(is_zero, is_255, allowed);
		cv::Mat invalid;
		cv::bitwise_not(allowed, invalid);

		if (cv::countNonZero(invalid) > 0)
		{
			return std::unexpected(
				"Input image is not binary (contains values other than 0/1). Enable preprocessing for non-binary images.");
		}

		return binarize(input);
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
				binary_image = preprocess_image(input_image);
				LOG(INFO) << "Preprocessed image for algorithm: " << algorithm_name;
			}
			else
			{
				const auto normalized = normalize_binary_image(input_image);
				if (!normalized)
				{
					return std::unexpected(normalized.error());
				}
				binary_image = normalized.value();
			}

			const algorithm_factory factory(processor_type_);
			const auto skeletonizer = factory.create(algorithm_name);

			if (!skeletonizer)
			{
				return std::unexpected("Unknown algorithm: " + algorithm_name);
			}

			skeletonizer->apply(binary_image);

			LOG(INFO) << "Skeletonization completed: " << algorithm_name << " (" << skeletonizer::to_string(
					processor_type_)
				<< ")";

			return scale(binary_image);
		}
		catch (const std::exception& e)
		{
			return std::unexpected(std::string{"Processing error: "} + e.what());
		}
	}
}
