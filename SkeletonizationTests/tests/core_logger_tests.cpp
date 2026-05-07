/**
 *
 * @file core_logger_tests.cpp
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Tests logging utilities.
 *
 * This file contains unit tests for logging helpers.
 *
 * @version 1.0
 * @date 2026-03-16
 */

#include "gtest/gtest.h"

#include "SkeletonizationCore/logger/logger.hpp"

TEST(Logger, InitializesWithoutThrow)
{
    EXPECT_NO_THROW({
        logger log{"SkeletonizationTests"};
        log.initialize();
    });
}

TEST(Logger, MultipleInstancesInitialize)
{
    EXPECT_DEATH({
        logger first{"SkeletonizationTests"};
        first.initialize();
        logger second{"SkeletonizationTests"};
        second.initialize();
    }, "");
}
