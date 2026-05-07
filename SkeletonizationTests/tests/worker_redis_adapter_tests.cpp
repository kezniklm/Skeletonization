/**
 *
 * @file worker_redis_adapter_tests.cpp
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Tests Redis adapter.
 *
 * This file contains unit tests for Redis adapter construction.
 *
 * @version 1.0
 * @date 2026-03-16
 */

#include "gtest/gtest.h"

#include <type_traits>

#include "SkeletonizationWorker/infrastructure/redis_adapter.hpp"

TEST(WorkerRedisAdapter, ConstructsWithClient)
{
    worker::infrastructure::redis::redis_configuration configuration;
    worker::infrastructure::redis::client client(configuration);

    worker::configuration::dependency_injection::jobs_queue_t jobs{"jobs"};
    worker::configuration::dependency_injection::processing_queue_t processing{"processing"};
    worker::configuration::dependency_injection::results_queue_t results{"results"};

    worker::infrastructure::redis_adapter adapter(client, jobs, processing, results);

    EXPECT_TRUE((std::is_base_of_v<worker::application::interfaces::i_jobs_queue,
                                  worker::infrastructure::redis_adapter>));
    EXPECT_TRUE((std::is_base_of_v<worker::application::interfaces::i_result_transport,
                                  worker::infrastructure::redis_adapter>));
    EXPECT_TRUE((std::is_base_of_v<worker::application::interfaces::i_job_status_store,
                                  worker::infrastructure::redis_adapter>));
}
