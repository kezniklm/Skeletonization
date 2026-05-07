/**
 *
 * @file worker_redis_client_tests.cpp
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Tests Redis client helpers.
 *
 * This file contains unit tests for Redis client connection info.
 *
 * @version 1.0
 * @date 2026-03-16
 */

#include "gtest/gtest.h"

#include "SkeletonizationWorker/infrastructure/redis.hpp"

TEST(WorkerRedisClient, ConnectionInfoUsesDefaults)
{
    worker::infrastructure::redis::redis_configuration configuration;
    configuration.uri.host = "redis-host";
    configuration.uri.port = 6379;
    configuration.uri.database = 1;
    configuration.uri.use_ssl = false;

    worker::infrastructure::redis::client client(configuration);

    EXPECT_EQ(client.connection_info(), "redis://redis-host:6379/1");
}

TEST(WorkerRedisClient, ConnectionInfoWithSsl)
{
    worker::infrastructure::redis::redis_configuration configuration;
    configuration.uri.host = "redis-host";
    configuration.uri.port = 6380;
    configuration.uri.database = 0;
    configuration.uri.use_ssl = true;

    worker::infrastructure::redis::client client(configuration);

    EXPECT_EQ(client.connection_info(), "rediss://redis-host:6380/0");
}
