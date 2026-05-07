/**
 *
 * @file worker_configuration_tests.cpp
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Tests worker configuration helpers.
 *
 * This file contains unit tests for worker configuration parsing.
 *
 * @version 1.0
 * @date 2026-03-16
 */

#include "gtest/gtest.h"

#include "SkeletonizationWorker/configuration.hpp"

using worker::configuration::backend::from_string;
using worker::configuration::backend::storage_backend;

TEST(WorkerConfiguration, ParsesStorageBackend)
{
    EXPECT_EQ(from_string("local"), storage_backend::local);
    EXPECT_EQ(from_string("s3"), storage_backend::s3);
    EXPECT_FALSE(from_string("unknown").has_value());
}
