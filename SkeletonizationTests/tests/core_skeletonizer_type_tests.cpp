/**
 *
 * @file core_skeletonizer_type_tests.cpp
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Tests skeletonizer type helpers.
 *
 * This file contains unit tests for skeletonizer type conversions.
 *
 * @version 1.0
 * @date 2026-03-16
 */

#include "gtest/gtest.h"

#include "SkeletonizationCore/skeletonizer/skeletonizer.hpp"

TEST(SkeletonizerType, ToStringViewMatches)
{
    EXPECT_EQ(skeletonizer::to_string_view(skeletonizer::skeletonizer_type::cpu), "cpu");
    EXPECT_EQ(skeletonizer::to_string_view(skeletonizer::skeletonizer_type::thread), "thread");
#if SKELETONIZATION_WITH_GPU
    EXPECT_EQ(skeletonizer::to_string_view(skeletonizer::skeletonizer_type::gpu), "gpu");
#endif
}

TEST(SkeletonizerType, ToStringUppercase)
{
    EXPECT_EQ(skeletonizer::to_string(skeletonizer::skeletonizer_type::cpu), "CPU");
    EXPECT_EQ(skeletonizer::to_string(skeletonizer::skeletonizer_type::thread), "THREAD");
#if SKELETONIZATION_WITH_GPU
    EXPECT_EQ(skeletonizer::to_string(skeletonizer::skeletonizer_type::gpu), "GPU");
#endif
}

TEST(SkeletonizerType, ToStringLowercase)
{
    EXPECT_EQ(skeletonizer::to_string(skeletonizer::skeletonizer_type::cpu, false), "cpu");
    EXPECT_EQ(skeletonizer::to_string(skeletonizer::skeletonizer_type::thread, false), "thread");
#if SKELETONIZATION_WITH_GPU
    EXPECT_EQ(skeletonizer::to_string(skeletonizer::skeletonizer_type::gpu, false), "gpu");
#endif
}

TEST(SkeletonizerType, FromStringParses)
{
    EXPECT_EQ(skeletonizer::from_string("cpu"), skeletonizer::skeletonizer_type::cpu);
    EXPECT_EQ(skeletonizer::from_string("CPU"), skeletonizer::skeletonizer_type::cpu);
    EXPECT_EQ(skeletonizer::from_string("thread"), skeletonizer::skeletonizer_type::thread);
    EXPECT_EQ(skeletonizer::from_string("THREAD"), skeletonizer::skeletonizer_type::thread);
#if SKELETONIZATION_WITH_GPU
    EXPECT_EQ(skeletonizer::from_string("gpu"), skeletonizer::skeletonizer_type::gpu);
    EXPECT_EQ(skeletonizer::from_string("GPU"), skeletonizer::skeletonizer_type::gpu);
#else
    EXPECT_FALSE(skeletonizer::from_string("gpu").has_value());
#endif
}

TEST(SkeletonizerType, FromStringRejectsUnknown)
{
    EXPECT_FALSE(skeletonizer::from_string("unknown").has_value());
}
