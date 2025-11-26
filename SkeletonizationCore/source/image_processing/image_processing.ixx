module;

#include <opencv2/opencv.hpp>

export module image_processing;

import pipeline;

constexpr auto default_ratio = 255;
constexpr auto high = 255;
constexpr auto low = 0;

export inline cv::Mat read_image(const std::string& path)
{
	const auto input_image = cv::imread(path);

	if (input_image.empty())
	{
		throw std::runtime_error("Failed to load image: " + path);
	}

	return input_image;
}

export inline cv::Mat convert_greyscale(const cv::Mat& image)
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

export inline cv::Mat blur(const cv::Mat& image)
{
	cv::Mat blurred_image = image.clone();

	cv::GaussianBlur(image, blurred_image, cv::Size(5, 5), 0);

	return blurred_image;
}

export inline cv::Mat threshold(const cv::Mat& image)
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
		10
	);

	return thresholded_image;
}

export inline cv::Mat clean_noise(const cv::Mat& image)
{
	cv::Mat cleaned;
	cv::morphologyEx(image, cleaned, cv::MORPH_CLOSE,
	                 cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)),
	                 cv::Point(-1, -1), 2);
	return cleaned;
}

export inline cv::Mat connect_components(const cv::Mat& image)
{
	cv::Mat connected;
	cv::dilate(image, connected,
	           cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));

	return connected;
}

export inline cv::Mat extract_filled_contours(const cv::Mat& image)
{
	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(image, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	cv::Mat filled = cv::Mat::zeros(image.size(), CV_8UC1);
	for (const auto& c : contours)
	{
		if (constexpr auto min_area = 100.0; cv::contourArea(c) > min_area)
		{
			cv::drawContours(filled, std::vector<std::vector<cv::Point>>{c}, -1, high, cv::FILLED);
		}
	}

	return filled;
}

export inline cv::Mat binarize(const cv::Mat& image)
{
	return image / default_ratio;
}

export inline cv::Mat scale(const cv::Mat& image)
{
	return image * default_ratio;
}

export inline cv::Mat preprocess_image(const cv::Mat& image)
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
