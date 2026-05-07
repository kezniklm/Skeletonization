/**
 *
 * @file worker_rapidjson_job_deserializer_tests.cpp
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Tests RapidJSON job deserialization.
 *
 * This file contains unit tests for worker job deserialization.
 *
 * @version 1.0
 * @date 2026-03-16
 */

#include "gtest/gtest.h"

#include "SkeletonizationWorker/infrastructure/rapidjson_job_deserializer.hpp"

using worker::infrastructure::rapidjson_job_deserializer;

TEST(WorkerRapidJsonJobDeserializer, ParsesValidPayload)
{
    rapidjson_job_deserializer deserializer;

    const std::string payload =
        R"({"id":"job-1","tasks":[{"image_key":"input.png","algorithm":"Zhang-Suen","should_run_preprocessing":true,"output_format":"PNG"}]})";

    const auto result = deserializer.deserialize(payload);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->id, "job-1");
    ASSERT_EQ(result->tasks.size(), 1u);
    EXPECT_EQ(result->tasks.front().path, "input.png");
    EXPECT_EQ(result->tasks.front().algorithm, "Zhang-Suen");
    EXPECT_TRUE(result->tasks.front().should_run_preprocessing);
    EXPECT_EQ(result->tasks.front().format, job::output_format::png);
}

TEST(WorkerRapidJsonJobDeserializer, RejectsMissingTasks)
{
    rapidjson_job_deserializer deserializer;

    const std::string payload = R"({"id":"job-1","tasks":[]})";

    const auto result = deserializer.deserialize(payload);

    EXPECT_FALSE(result.has_value());
}
