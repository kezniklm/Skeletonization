/**
 *
 * @file worker_s3_object_storage_tests.cpp
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Tests S3 object storage.
 *
 * This file contains unit tests for S3 object storage helpers.
 *
 * @version 1.0
 * @date 2026-03-16
 */

#include "gtest/gtest.h"

#include "SkeletonizationWorker/infrastructure/s3_object_storage.hpp"

TEST(WorkerS3ObjectStorage, ReportsRemoteBackend)
{
    worker::configuration::s3_configuration configuration;
    configuration.bucket = "bucket";
    configuration.access_key_id = "key";
    configuration.secret_access_key = "secret";

    worker::infrastructure::aws_sdk_initializer aws;
    worker::infrastructure::s3_object_storage storage(aws, configuration);

    EXPECT_TRUE(storage.is_remote_backend());
}
