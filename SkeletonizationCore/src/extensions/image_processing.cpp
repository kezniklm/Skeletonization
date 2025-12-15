#include <stdexcept>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

#include "SkeletonizationCore/extensions/image_processing.hpp"
#include "SkeletonizationCore/extensions/pipeline.hpp"

cv::Mat read_image(const std::string& path)
{
	const auto input_image = cv::imread(path);

	if (input_image.empty())
	{
		throw std::runtime_error("Failed to load image: " + path);
	}

	return input_image;
}

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

cv::Mat blur(const cv::Mat& image)
{
	cv::Mat blurred_image = image.clone();

	cv::GaussianBlur(image, blurred_image, cv::Size(5, 5), 0);

	return blurred_image;
}

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

cv::Mat connect_components(const cv::Mat& image)
{
	cv::Mat connected;
	cv::dilate(
		image,
		connected,
		cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));

	return connected;
}

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

cv::Mat binarize(const cv::Mat& image)
{
	return image / default_ratio;
}

cv::Mat scale(const cv::Mat& image)
{
	return image * default_ratio;
}

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
