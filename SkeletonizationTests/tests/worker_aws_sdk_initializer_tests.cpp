/**
 *
 * @file worker_aws_sdk_initializer_tests.cpp
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Tests AWS SDK initializer.
 *
 * This file contains unit tests for AWS SDK lifecycle helpers.
 *
 * @version 1.0
 * @date 2026-03-16
 */

#include "gtest/gtest.h"

#include "SkeletonizationWorker/infrastructure/aws_sdk_initializer.hpp"

TEST(WorkerAwsSdkInitializer, ConstructsAndDestructs)
{
    EXPECT_NO_THROW({
        worker::infrastructure::aws_sdk_initializer initializer;
    });
}
