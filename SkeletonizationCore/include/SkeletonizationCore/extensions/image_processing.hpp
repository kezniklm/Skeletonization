#pragma once

#include <string>

#include "opencv2/core/mat.hpp"

constexpr auto default_ratio = 255;
constexpr auto high = 255;
constexpr auto low = 0;

cv::Mat read_image(const std::string& path);
cv::Mat convert_greyscale(const cv::Mat& image);
cv::Mat blur(const cv::Mat& image);
cv::Mat threshold(const cv::Mat& image);
cv::Mat clean_noise(const cv::Mat& image);
cv::Mat connect_components(const cv::Mat& image);
cv::Mat extract_filled_contours(const cv::Mat& image);
cv::Mat binarize(const cv::Mat& image);
cv::Mat scale(const cv::Mat& image);
cv::Mat preprocess_image(const cv::Mat& image);
