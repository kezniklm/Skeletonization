/**
 *
 * @file core_image_processing_tests.cpp
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Tests image processing utilities.
 *
 * This file contains unit tests for preprocessing and image helpers.
 *
 * @version 1.0
 * @date 2026-03-16
 */

#include "gtest/gtest.h"

#include "SkeletonizationCore/extensions/image_processing.hpp"

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

TEST(ImageProcessing, BinarizeAndScaleAreInverse)
{
    cv::Mat input(2, 2, CV_8UC1);
    input.at<uchar>(0, 0) = 0;
    input.at<uchar>(0, 1) = 255;
    input.at<uchar>(1, 0) = 255;
    input.at<uchar>(1, 1) = 0;

    const auto bin = binarize(input);
    const auto scaled = scale(bin);

    EXPECT_EQ(cv::countNonZero(input != scaled), 0);
}

TEST(ImageProcessing, ConvertGreyscalePreservesDimensions)
{
    cv::Mat input(3, 4, CV_8UC3, cv::Scalar(10, 20, 30));
    const auto gray = convert_greyscale(input);

    EXPECT_EQ(gray.rows, input.rows);
    EXPECT_EQ(gray.cols, input.cols);
    EXPECT_EQ(gray.channels(), 1);
}

TEST(ImageProcessing, ThresholdProducesBinary)
{
    cv::Mat input(5, 5, CV_8UC1, cv::Scalar(128));
    const auto output = threshold(input);

    double min_val = 0.0;
    double max_val = 0.0;
    cv::minMaxLoc(output, &min_val, &max_val);

    EXPECT_TRUE((min_val == 0.0 || min_val == 255.0));
    EXPECT_TRUE((max_val == 0.0 || max_val == 255.0));
}

TEST(ImageProcessing, CleanNoiseKeepsSize)
{
    cv::Mat input(10, 10, CV_8UC1, cv::Scalar(0));
    cv::rectangle(input, {2, 2}, {7, 7}, cv::Scalar(255), cv::FILLED);

    const auto cleaned = clean_noise(input);

    EXPECT_EQ(cleaned.rows, input.rows);
    EXPECT_EQ(cleaned.cols, input.cols);
}
