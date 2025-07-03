module;

#include <opencv2/opencv.hpp>

export module image_processing;

export inline cv::Mat binarize(const cv::Mat &image, const double threshold)
{
    constexpr double maximal_value = 255.0;

    cv::Mat binary;

    cv::threshold(image, binary, threshold, maximal_value, cv::THRESH_BINARY);

    return binary;
}