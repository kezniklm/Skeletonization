/**
 *
 * @file core_class_name_tests.cpp
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Tests class name reflection helpers.
 *
 * This file contains unit tests for extracting class names.
 *
 * @version 1.0
 * @date 2026-03-16
 */

#include "gtest/gtest.h"

#include "SkeletonizationCore/reflection/class_name.hpp"

namespace
{
    struct local_type
    {
    };

    namespace nested
    {
        struct inner_type
        {
        };
    }
}

TEST(Reflection, ClassNameStripsNamespace)
{
    EXPECT_EQ(class_name<local_type>(), "local_type");
}

TEST(Reflection, ClassNameEqualityForLocalType)
{
    const std::string_view first = class_name<local_type>();
    const std::string_view second = class_name<local_type>();
    EXPECT_EQ(first, second);
}

TEST(Reflection, ClassNameForStdType)
{
 const std::string_view name = class_name<std::string>();
 EXPECT_FALSE(name.empty());
}

TEST(Reflection, ClassNameForNestedNamespace)
{
    EXPECT_EQ(class_name<nested::inner_type>(), "inner_type");
}
