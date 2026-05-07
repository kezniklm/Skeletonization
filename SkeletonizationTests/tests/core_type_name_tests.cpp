/**
 *
 * @file core_type_name_tests.cpp
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Tests type name reflection helpers.
 *
 * This file contains unit tests for type name extraction.
 *
 * @version 1.0
 * @date 2026-03-16
 */

#include "gtest/gtest.h"

#include "SkeletonizationCore/reflection/type_name.hpp"

namespace
{
    struct local_type
    {
    };

    struct another_type
    {
    };
}

TEST(Reflection, TypeNameContainsIdentifier)
{
    const std::string_view name = type_name<local_type>();
    EXPECT_NE(name.find("local_type"), std::string_view::npos);
}

TEST(Reflection, TypeNameEqualityForLocalType)
{
    const std::string_view first = type_name<local_type>();
    const std::string_view second = type_name<local_type>();
    EXPECT_EQ(first, second);
}

TEST(Reflection, TypeNameContainsOtherIdentifier)
{
    const std::string_view name = type_name<another_type>();
    EXPECT_NE(name.find("another_type"), std::string_view::npos);
}
