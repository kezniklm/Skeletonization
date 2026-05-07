/**
 *
 * @file worker_image_service_tests.cpp
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Tests worker image service.
 *
 * This file contains unit tests for image loading and saving.
 *
 * @version 1.0
 * @date 2026-03-16
 */

#include "gtest/gtest.h"

#include <filesystem>

#include "opencv2/imgcodecs.hpp"

#include "SkeletonizationWorker/infrastructure/image_service.hpp"

namespace
{
    std::filesystem::path make_temp_path(const std::string& suffix)
    {
        const auto base = std::filesystem::temp_directory_path();
        return base / ("skeleton_worker_image_service_" + suffix);
    }
}

TEST(WorkerImageService, LoadImageFailsForMissingFile)
{
    worker::infrastructure::image_service service;
    const auto missing = make_temp_path("missing.png");

    const auto result = service.load_image(missing.string());

    EXPECT_FALSE(result.has_value());
}

TEST(WorkerImageService, SaveAndLoadRoundtrip)
{
    worker::infrastructure::image_service service;

    const auto path = make_temp_path("roundtrip.png");

    cv::Mat image(4, 4, CV_8UC1, cv::Scalar(128));

    const auto save_result = service.save_image(image, path.string());
    ASSERT_TRUE(save_result.has_value());

    const auto load_result = service.load_image(path.string());
    ASSERT_TRUE(load_result.has_value());
    EXPECT_EQ(load_result->rows, image.rows);
    EXPECT_EQ(load_result->cols, image.cols);

    std::error_code ec;
    std::filesystem::remove(path, ec);
}
