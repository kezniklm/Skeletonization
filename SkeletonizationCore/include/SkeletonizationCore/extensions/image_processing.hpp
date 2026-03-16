/**
*
* @file image_processing.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares image preprocessing utilities for skeletonization input.
*
* This header defines reusable image preprocessing steps that transform loaded
* images into binary inputs suitable for skeletonization algorithms.
*
* Main responsibilities:
* - load image data from file paths
* - apply grayscale, denoising, and threshold preprocessing steps
* - produce normalized binary matrices for algorithm input
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <expected>
#include <string>

#include "opencv2/core/mat.hpp"

/// Default scale ratio for binary value normalization.
constexpr auto default_ratio = 255;
/// High pixel value used for binary operations.
constexpr auto high = 255;
/// Low pixel value used for binary operations.
constexpr auto low = 0;

/**
 * @brief Reads an image from disk.
 *
 * @param path Image file path.
 * @return Loaded image or error message.
 */
std::expected<cv::Mat, std::string> read_image(const std::string& path);
/**
 * @brief Converts an image to grayscale.
 *
 * @param image Source image.
 * @return Grayscale image.
 */
cv::Mat convert_greyscale(const cv::Mat& image);
/**
 * @brief Applies gaussian blur to reduce noise.
 *
 * @param image Source image.
 * @return Blurred image.
 */
cv::Mat blur(const cv::Mat& image);
/**
 * @brief Applies adaptive thresholding to produce a binary mask.
 *
 * @param image Source grayscale image.
 * @return Thresholded binary image.
 */
cv::Mat threshold(const cv::Mat& image);
/**
 * @brief Performs morphological cleanup on binary noise.
 *
 * @param image Source binary image.
 * @return Noise-cleaned image.
 */
cv::Mat clean_noise(const cv::Mat& image);
/**
 * @brief Connects nearby foreground components.
 *
 * @param image Source binary image.
 * @return Dilated binary image.
 */
cv::Mat connect_components(const cv::Mat& image);
/**
 * @brief Extracts and fills external contours above area threshold.
 *
 * @param image Source binary image.
 * @return Filled contour image.
 */
cv::Mat extract_filled_contours(const cv::Mat& image);
/**
 * @brief Converts binary 0/255 image to 0/1 representation.
 *
 * @param image Source binary image.
 * @return Normalized binary image.
 */
cv::Mat binarize(const cv::Mat& image);
/**
 * @brief Converts binary 0/1 image to 0/255 representation.
 *
 * @param image Source normalized binary image.
 * @return Scaled binary image.
 */
cv::Mat scale(const cv::Mat& image);
/**
 * @brief Runs the full preprocessing pipeline.
 *
 * @param image Source image.
 * @return Preprocessed binary image.
 */
cv::Mat preprocess_image(const cv::Mat& image);
