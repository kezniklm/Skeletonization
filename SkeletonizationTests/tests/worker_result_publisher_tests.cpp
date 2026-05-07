/**
 *
 * @file worker_result_publisher_tests.cpp
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Tests result publishing helpers.
 *
 * This file contains unit tests for result publisher payloads.
 *
 * @version 1.0
 * @date 2026-03-16
 */

#include "gtest/gtest.h"

#include <string>

#include "rapidjson/document.h"

#include "SkeletonizationWorker/infrastructure/result_publisher.hpp"

namespace
{
    class fake_result_transport final : public worker::application::interfaces::i_result_transport
    {
    public:
        std::expected<void, std::string> publish_result(const std::string& result_payload) override
        {
            last_payload = result_payload;
            return {};
        }

        std::string last_payload;
    };
}

TEST(WorkerResultPublisher, PublishesSuccessPayload)
{
    fake_result_transport transport;

    worker::configuration::dependency_injection::worker_id_t worker_id{"worker-1"};

    worker::infrastructure::result_publisher publisher(
        transport,
        worker_id,
        skeletonizer::skeletonizer_type::cpu);

    const auto result = publisher.publish_task_result(
        "job-1",
        "image.png",
        "Zhang-Suen",
        "outputs/result.png",
        true,
        12.5);

    ASSERT_TRUE(result.has_value());
    ASSERT_FALSE(transport.last_payload.empty());

    rapidjson::Document document;
    document.Parse(transport.last_payload.c_str());

    ASSERT_TRUE(document.IsObject());
    EXPECT_STREQ(document["job_id"].GetString(), "job-1");
    EXPECT_STREQ(document["worker_id"].GetString(), "worker-1");
    EXPECT_STREQ(document["worker_type"].GetString(), "cpu");
    EXPECT_STREQ(document["image_path"].GetString(), "image.png");
    EXPECT_STREQ(document["algorithm"].GetString(), "Zhang-Suen");
    EXPECT_STREQ(document["output_path"].GetString(), "outputs/result.png");
    EXPECT_TRUE(document["success"].GetBool());
}

TEST(WorkerResultPublisher, PublishesFailurePayloadWithError)
{
    fake_result_transport transport;

    worker::configuration::dependency_injection::worker_id_t worker_id{"worker-1"};

    worker::infrastructure::result_publisher publisher(
        transport,
        worker_id,
        skeletonizer::skeletonizer_type::thread);

    const auto result = publisher.publish_task_result(
        "job-2",
        "image.png",
        "Zhang-Suen",
        "",
        false,
        2.1,
        "error");

    ASSERT_TRUE(result.has_value());
    ASSERT_FALSE(transport.last_payload.empty());

    rapidjson::Document document;
    document.Parse(transport.last_payload.c_str());

    ASSERT_TRUE(document.IsObject());
    EXPECT_STREQ(document["worker_type"].GetString(), "mt");
    EXPECT_STREQ(document["error"].GetString(), "error");
    EXPECT_STREQ(document["error_message"].GetString(), "error");
    EXPECT_FALSE(document["success"].GetBool());
}
