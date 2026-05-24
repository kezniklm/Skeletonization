/**
*
* @file standard_image_processor.cpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Implements standard image processing operations.
*
* This file implements the preprocessing and normalization operations
* used for skeletonization input preparation.
*
* Main responsibilities:
* - run standard preprocessing pipeline
* - normalize binary inputs for algorithm execution
*
* @version 1.0
* @date 2026-03-16
*/

#include "SkeletonizationCore/extensions/standard_image_processor.hpp"

#include "SkeletonizationCore/extensions/image_processing.hpp"

cv::Mat standard_image_processor::preprocess(const cv::Mat& image) const
{
	return preprocess_image(image);
}

std::expected<cv::Mat, std::string> standard_image_processor::normalize_binary_image(const cv::Mat& input) const
{
	if (input.empty())
	{
		return std::unexpected("Input image is empty");
	}

	cv::Mat single_channel = input;
	if (input.channels() != 1)
	{
		if (input.channels() == 3 || input.channels() == 4)
		{
			single_channel = convert_greyscale(input);
		}
		else
		{
			std::vector<cv::Mat> channels;
			cv::split(input, channels);
			if (channels.empty())
			{
				return std::unexpected(
					"Input image has no channels available for binary normalization. Enable preprocessing for other formats.");
			}
			single_channel = channels.front();
		}
	}

	cv::Mat is_zero;
	cv::Mat is_one;
	cv::Mat is_255;
	cv::compare(single_channel, 0, is_zero, cv::CMP_EQ);
	cv::compare(single_channel, 1, is_one, cv::CMP_EQ);
	cv::compare(single_channel, 255, is_255, cv::CMP_EQ);

	cv::Mat allowed;
	cv::bitwise_or(is_zero, is_one, allowed);
	cv::bitwise_or(allowed, is_255, allowed);

	cv::Mat invalid;
	cv::bitwise_not(allowed, invalid);

	if (cv::countNonZero(invalid) == 0)
	{
		cv::Mat normalized = single_channel != 0;
		normalized /= 255;
		return normalized;
	}

	double min_val = 0.0;
	double max_val = 0.0;

	cv::minMaxLoc(single_channel, &min_val, &max_val);

	if (min_val >= 0.0 && max_val <= 1.0)
	{
		return std::unexpected(
			"Input image is not binary (expected only 0/1 values). Enable preprocessing to convert the image before running algorithms.");
	}

	if (min_val >= 0.0 && max_val <= 255.0)
	{
		return single_channel / 255.0;
	}

	return std::unexpected(
		"Input image is not binary (expected 0/1 or 0/255). Enable preprocessing to convert the image before running algorithms.");
}
