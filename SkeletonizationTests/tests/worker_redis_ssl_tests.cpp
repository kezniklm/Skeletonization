/**
 *
 * @file worker_redis_ssl_tests.cpp
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Tests Redis SSL helpers.
 *
 * This file contains unit tests for Redis SSL initialization.
 *
 * @version 1.0
 * @date 2026-03-16
 */

#include "gtest/gtest.h"

#include "SkeletonizationWorker/infrastructure/redis_ssl.hpp"

using worker::infrastructure::redis::ssl_library_initializer;
using worker::infrastructure::redis::ssl_context;

TEST(WorkerRedisSsl, InitializesLibraryOnce)
{
    const auto result = ssl_library_initializer::initialize();
    EXPECT_TRUE(result.has_value());
    EXPECT_TRUE(ssl_library_initializer::is_initialized());
}

TEST(WorkerRedisSsl, AppliesWithoutContextFails)
{
    ssl_context ctx;

    const auto result = ctx.apply_to_context(nullptr);

    EXPECT_FALSE(result.has_value());
}
