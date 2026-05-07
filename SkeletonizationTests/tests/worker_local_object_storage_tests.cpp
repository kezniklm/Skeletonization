/**
 *
 * @file worker_local_object_storage_tests.cpp
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Tests local object storage.
 *
 * This file contains unit tests for local object storage operations.
 *
 * @version 1.0
 * @date 2026-03-16
 */

#include "gtest/gtest.h"

#include <filesystem>
#include <fstream>

#include "SkeletonizationWorker/infrastructure/local_object_storage.hpp"

namespace
{
    std::filesystem::path make_temp_path(const std::string& suffix)
    {
        const auto base = std::filesystem::temp_directory_path();
        return base / ("skeleton_worker_local_object_storage_" + suffix);
    }
}

TEST(WorkerLocalObjectStorage, DownloadCopiesFile)
{
    worker::infrastructure::local_object_storage storage;

    const auto source = make_temp_path("source.txt");
    const auto destination = make_temp_path("downloaded.txt");

    {
        std::ofstream output(source);
        output << "payload";
    }

    const auto result = storage.download_to_file(source.string(), destination);
    ASSERT_TRUE(result.has_value());

    std::ifstream input(destination);
    std::string payload;
    input >> payload;
    EXPECT_EQ(payload, "payload");

    std::error_code ec;
    std::filesystem::remove(source, ec);
    std::filesystem::remove(destination, ec);
}

TEST(WorkerLocalObjectStorage, UploadCopiesFile)
{
    worker::infrastructure::local_object_storage storage;

    const auto source = make_temp_path("upload_source.txt");
    const auto destination = make_temp_path("uploads/uploaded.txt");

    {
        std::ofstream output(source);
        output << "upload";
    }

    const auto result = storage.upload_from_file(source, destination.string(), {});
    ASSERT_TRUE(result.has_value());

    std::ifstream input(destination);
    std::string payload;
    input >> payload;
    EXPECT_EQ(payload, "upload");

    std::error_code ec;
    std::filesystem::remove(source, ec);
    std::filesystem::remove(destination, ec);
    std::filesystem::remove(destination.parent_path(), ec);
}

TEST(WorkerLocalObjectStorage, IsNotRemote)
{
    worker::infrastructure::local_object_storage storage;
    EXPECT_FALSE(storage.is_remote_backend());
}
