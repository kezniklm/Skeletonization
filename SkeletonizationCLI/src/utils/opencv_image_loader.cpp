/**
*
* @file opencv_image_loader.cpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Implements OpenCV-backed image loading.
*
* This file implements image loading through OpenCV filesystem APIs.
*
* Main responsibilities:
* - load image files into cv::Mat values
* - validate load success and report failures
* - provide default loader implementation behavior
*
* @version 1.0
* @date 2026-03-16
*/

#include "SkeletonizationCLI/utils/opencv_image_loader.hpp"

#include "SkeletonizationCore/extensions/image_processing.hpp"
#include "SkeletonizationCLI/exceptions/export_exception.hpp"

namespace cli::utils
{
	cv::Mat opencv_image_loader::load(const std::filesystem::path& path) const
	{
		const auto result = read_image(path.string());

		if (!result)
		{
			throw exceptions::image_loading_exception(path, result.error());
		}

		return result.value();
	}
}
