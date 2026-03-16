/**
*
* @file image_processing.cpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Implements preprocessing steps for skeletonization input images.
*
* This file provides concrete OpenCV-based transformations used to prepare
* input images for thinning algorithms. It includes loading, cleanup, contour
* extraction, and binary normalization operations.
*
* Main responsibilities:
* - load images and validate input availability
* - apply preprocessing transformations for noise and structure cleanup
* - produce binary matrices ready for skeletonization execution
*
* @version 1.0
* @date 2026-03-16
*/

#include <expected>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

#include "SkeletonizationCore/extensions/image_processing.hpp"
#include "SkeletonizationCore/extensions/pipeline.hpp"

/**
 * @brief Reads an image from disk.
 *
 * @param path Image file path.
 * @return Loaded image or failure message.
 */
std::expected<cv::Mat, std::string> read_image(const std::string& path)
{
	auto input_image = cv::imread(path);

	if (input_image.empty())
	{
		return std::unexpected("Failed to load image: " + path);
	}

	return input_image;
}

/**
 * @brief Converts input image to grayscale format.
 *
 * @param image Source image.
 * @return Grayscale image.
 */
cv::Mat convert_greyscale(const cv::Mat& image)
{
	cv::Mat gray_image = image.clone();

	if (image.channels() == 4)
	{
		cv::cvtColor(image, gray_image, cv::COLOR_BGRA2GRAY);
	}
	else
	{
		cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);
	}

	return gray_image;
}

/**
 * @brief Applies gaussian blur for noise reduction.
 *
 * @param image Source image.
 * @return Blurred image.
 */
cv::Mat blur(const cv::Mat& image)
{
	cv::Mat blurred_image = image.clone();

	cv::GaussianBlur(image, blurred_image, cv::Size(5, 5), 0);

	return blurred_image;
}

/**
 * @brief Applies adaptive thresholding with inversion.
 *
 * @param image Source grayscale image.
 * @return Thresholded binary image.
 */
cv::Mat threshold(const cv::Mat& image)
{
	constexpr auto maximal_value = 255.0;

	cv::Mat thresholded_image;
	cv::adaptiveThreshold(
		image,
		thresholded_image,
		maximal_value,
		cv::ADAPTIVE_THRESH_GAUSSIAN_C,
		cv::THRESH_BINARY_INV,
		45,
		10);

	return thresholded_image;
}

/**
 * @brief Removes small noise using morphological closing.
 *
 * @param image Source binary image.
 * @return Morphologically cleaned image.
 */
cv::Mat clean_noise(const cv::Mat& image)
{
	cv::Mat cleaned;

	cv::morphologyEx(
		image,
		cleaned,
		cv::MORPH_CLOSE,
		cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)),
		cv::Point(-1, -1),
		2);

	return cleaned;
}

/**
 * @brief Connects nearby binary components using dilation.
 *
 * @param image Source binary image.
 * @return Dilated image.
 */
cv::Mat connect_components(const cv::Mat& image)
{
	cv::Mat connected;

	cv::dilate(
		image,
		connected,
		cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));

	return connected;
}

/**
 * @brief Extracts and fills external contours larger than a minimum area.
 *
 * @param image Source binary image.
 * @return Filled contour mask.
 */
cv::Mat extract_filled_contours(const cv::Mat& image)
{
	std::vector<std::vector<cv::Point>> contours;

	cv::findContours(
		image,
		contours,
		cv::RETR_EXTERNAL,
		cv::CHAIN_APPROX_SIMPLE);

	cv::Mat filled = cv::Mat::zeros(image.size(), CV_8UC1);

	for (const auto& c : contours)
	{
		if (constexpr auto min_area = 100.0; cv::contourArea(c) > min_area)
		{
			cv::drawContours(
				filled,
				std::vector<std::vector<cv::Point>>{c},
				-1,
				high,
				cv::FILLED);
		}
	}

	return filled;
}

/**
 * @brief Normalizes binary values from 0/255 to 0/1.
 *
 * @param image Source binary image.
 * @return Normalized binary image.
 */
cv::Mat binarize(const cv::Mat& image)
{
	return image / default_ratio;
}

/**
 * @brief Scales binary values from 0/1 to 0/255.
 *
 * @param image Source normalized image.
 * @return Scaled binary image.
 */
cv::Mat scale(const cv::Mat& image)
{
	return image * default_ratio;
}

/**
 * @brief Runs the full preprocessing pipeline.
 *
 * @param image Source image.
 * @return Preprocessed binary image.
 */
cv::Mat preprocess_image(const cv::Mat& image)
{
	return pipeline(image)
	       .then(convert_greyscale)
	       .then(blur)
	       .then(threshold)
	       .then(clean_noise)
	       .then(connect_components)
	       .then(clean_noise)
	       .then(extract_filled_contours)
	       .then(binarize)
	       .get();
}
