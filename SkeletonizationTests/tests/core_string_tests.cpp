/**
 *
 * @file core_string_tests.cpp
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Tests string utilities.
 *
 * This file contains unit tests for ASCII string helpers.
 *
 * @version 1.0
 * @date 2026-03-16
 */

#include "gtest/gtest.h"

#include "SkeletonizationCore/extensions/string.hpp"

TEST(StringExtensions, ToLowerConverts)
{
    EXPECT_EQ(to_lower("ABC"), "abc");
    EXPECT_EQ(to_lower("MiXeD_123"), "mixed_123");
}

TEST(StringExtensions, EqualsAsciiIgnoresCase)
{
    EXPECT_TRUE(equals_ascii("Test", "test"));
    EXPECT_TRUE(equals_ascii("JPEG", "jpeg"));
    EXPECT_FALSE(equals_ascii("PNG", "JPG"));
}

TEST(StringExtensions, EqualsAsciiRejectsDifferentLength)
{
    EXPECT_FALSE(equals_ascii("test", "tests"));
}
