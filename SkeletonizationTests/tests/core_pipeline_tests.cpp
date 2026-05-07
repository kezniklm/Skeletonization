/**
 *
 * @file core_pipeline_tests.cpp
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Tests pipeline utilities.
 *
 * This file contains unit tests for pipeline chaining helpers.
 *
 * @version 1.0
 * @date 2026-03-16
 */

#include "gtest/gtest.h"

#include "SkeletonizationCore/extensions/pipeline.hpp"

TEST(Pipeline, ChainsTransformations)
{
    auto result = pipeline{2}
        .then([](int value) { return value + 3; })
        .then([](int value) { return value * 4; })
        .get();

    EXPECT_EQ(result, 20);
}

TEST(Pipeline, WorksWithMoveOnlyTypes)
{
    auto result = pipeline{std::make_unique<int>(7)}
        .then([](std::unique_ptr<int> value)
        {
            *value += 5;
            return value;
        })
        .get();

    ASSERT_NE(result, nullptr);
    EXPECT_EQ(*result, 12);
}
