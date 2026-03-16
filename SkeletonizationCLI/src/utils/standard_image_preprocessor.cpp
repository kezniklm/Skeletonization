/**
*
* @file standard_image_preprocessor.cpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Implements standard image preprocessing steps.
*
* This file implements default image preprocessing pipeline stages.
*
* Main responsibilities:
* - apply preprocessing operations to input images
* - normalize outputs for benchmark execution
* - provide reusable preprocessor implementation
*
* @version 1.0
* @date 2026-03-16
*/

#include "SkeletonizationCLI/utils/standard_image_preprocessor.hpp"

#include "SkeletonizationCore/extensions/image_processing.hpp"

namespace cli::utils
{
	cv::Mat standard_image_preprocessor::preprocess(const cv::Mat& image) const
	{
		return preprocess_image(image);
	}
}
