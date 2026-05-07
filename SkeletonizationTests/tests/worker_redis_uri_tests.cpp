/**
 *
 * @file worker_redis_uri_tests.cpp
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Tests Redis URI parsing.
 *
 * This file contains unit tests for Redis URI parsing helpers.
 *
 * @version 1.0
 * @date 2026-03-16
 */

#include "gtest/gtest.h"

#include "SkeletonizationWorker/infrastructure/redis_uri.hpp"

using worker::infrastructure::redis::redis_uri_parser;
using worker::infrastructure::redis::redis_uri_components;
using worker::infrastructure::redis::redis_configuration;

TEST(WorkerRedisUri, ParsesBasicRedisUri)
{
    const auto result = redis_uri_parser::parse("redis://localhost:6380/2");
    ASSERT_TRUE(result.has_value());

    const auto& parsed = result.value();
    EXPECT_EQ(parsed.host, "localhost");
    EXPECT_EQ(parsed.port, 6380);
    EXPECT_EQ(parsed.database, 2);
    EXPECT_FALSE(parsed.use_ssl);
}

TEST(WorkerRedisUri, ParsesRedisUriWithCredentials)
{
    const auto result = redis_uri_parser::parse("redis://user:pass@host:6379/0");
    ASSERT_TRUE(result.has_value());

    const auto& parsed = result.value();
    EXPECT_EQ(parsed.username, "user");
    EXPECT_EQ(parsed.password, "pass");
    EXPECT_EQ(parsed.host, "host");
}

TEST(WorkerRedisUri, ParsesRedisUriWithPasswordOnly)
{
    const auto result = redis_uri_parser::parse("redis://secret@host:6379/0");
    ASSERT_TRUE(result.has_value());

    const auto& parsed = result.value();
    EXPECT_TRUE(parsed.username.empty());
    EXPECT_EQ(parsed.password, "secret");
}

TEST(WorkerRedisUri, ParsesRedissScheme)
{
    const auto result = redis_uri_parser::parse("rediss://host:6379/0");
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result->use_ssl);
}

TEST(WorkerRedisUri, RejectsInvalidScheme)
{
    const auto result = redis_uri_parser::parse("http://host:6379/0");
    EXPECT_FALSE(result.has_value());
}

TEST(WorkerRedisUri, RejectsInvalidPort)
{
    const auto result = redis_uri_parser::parse("redis://host:99999/0");
    EXPECT_FALSE(result.has_value());
}

TEST(WorkerRedisUri, RejectsInvalidDatabase)
{
    const auto result = redis_uri_parser::parse("redis://host:6379/42");
    EXPECT_FALSE(result.has_value());
}

TEST(WorkerRedisUri, ConfigurationFromUrl)
{
    const auto config = redis_configuration::from_url("redis://localhost:6379/1");
    ASSERT_TRUE(config.has_value());
    EXPECT_EQ(config->uri.database, 1);
}
