/**
 *
 * @file worker_skeletonization_processor_tests.cpp
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Tests skeletonization processor.
 *
 * This file contains unit tests for skeletonization processing.
 *
 * @version 1.0
 * @date 2026-03-16
 */

#include "gtest/gtest.h"

#include "opencv2/core.hpp"

#include "SkeletonizationWorker/infrastructure/skeletonization_processor.hpp"

TEST(WorkerSkeletonizationProcessor, RejectsEmptyImage)
{
    worker::infrastructure::skeletonization_processor processor;

    const cv::Mat image;

    const auto result = processor.process(image, "Zhang-Suen", false);

    EXPECT_FALSE(result.has_value());
}

TEST(WorkerSkeletonizationProcessor, PreprocessesAndProcesses)
{
    worker::infrastructure::skeletonization_processor processor;

    cv::Mat image(10, 10, CV_8UC3, cv::Scalar(255, 255, 255));
    image.at<cv::Vec3b>(0, 0) = cv::Vec3b(0, 0, 0);

    const auto result = processor.process(image, "Zhang-Suen", true);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->rows, image.rows);
    EXPECT_EQ(result->cols, image.cols);
}

TEST(WorkerSkeletonizationProcessor, ReportsUnknownAlgorithm)
{
    worker::infrastructure::skeletonization_processor processor;

    cv::Mat image(5, 5, CV_8UC1, cv::Scalar(0));

    const auto result = processor.process(image, "Unknown", true);

    EXPECT_FALSE(result.has_value());
}
