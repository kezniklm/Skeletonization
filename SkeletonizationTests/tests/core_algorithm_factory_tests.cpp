/**
 *
 * @file core_algorithm_factory_tests.cpp
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Tests the skeletonizer algorithm factory.
 *
 * This file contains unit tests for algorithm discovery and creation.
 *
 * @version 1.0
 * @date 2026-03-16
 */

#include "gtest/gtest.h"

#include <string>

#include "SkeletonizationCore/skeletonizer/algorithm_factory.hpp"

TEST(AlgorithmFactory, ListsAlgorithms)
{
    const auto algorithms = skeletonizer::algorithm_factory::available_algorithms();
    EXPECT_FALSE(algorithms.empty());
}

TEST(AlgorithmFactory, CreateCpuAlgorithm)
{
    const auto algorithms = skeletonizer::algorithm_factory::available_algorithms();
    ASSERT_FALSE(algorithms.empty());

    const auto algorithm_name = std::string(algorithms.front());
    const auto instance = skeletonizer::algorithm_factory::create(algorithm_name, skeletonizer::skeletonizer_type::cpu);
    ASSERT_NE(instance, nullptr);
    EXPECT_FALSE(instance->name().empty());
}

TEST(AlgorithmFactory, CreateWithNormalizedName)
{
    const auto algorithms = skeletonizer::algorithm_factory::available_algorithms();
    ASSERT_FALSE(algorithms.empty());

    std::string algorithm_name = std::string(algorithms.front());
    for (const auto& candidate : algorithms)
    {
        if (candidate.find('_') != std::string_view::npos)
        {
            algorithm_name = std::string(candidate);
            break;
        }
    }

    std::string normalized = algorithm_name;
    std::ranges::replace(normalized, '_', '-');
    const auto instance = skeletonizer::algorithm_factory::create(normalized, skeletonizer::skeletonizer_type::cpu);
    ASSERT_NE(instance, nullptr);
}

TEST(AlgorithmFactory, SupportsCpuAlgorithms)
{
    const auto algorithms = skeletonizer::algorithm_factory::available_algorithms();
    ASSERT_FALSE(algorithms.empty());

    const auto algorithm_name = std::string(algorithms.front());
    EXPECT_TRUE(skeletonizer::algorithm_factory::supports(algorithm_name, skeletonizer::skeletonizer_type::cpu));
}

TEST(AlgorithmFactory, SupportsThreadedAlgorithms)
{
    const auto algorithms = skeletonizer::algorithm_factory::available_algorithms();
    ASSERT_FALSE(algorithms.empty());

    const auto algorithm_name = std::string(algorithms.front());
    EXPECT_TRUE(skeletonizer::algorithm_factory::supports(algorithm_name, skeletonizer::skeletonizer_type::thread));
}

TEST(AlgorithmFactory, CreatorsForAlgorithmReturnsList)
{
    const auto algorithms = skeletonizer::algorithm_factory::available_algorithms();
    ASSERT_FALSE(algorithms.empty());

    const auto algorithm_name = std::string(algorithms.front());
    const auto creators = skeletonizer::algorithm_factory::creators_for(
        algorithm_name,
        skeletonizer::skeletonizer_type::cpu);
    EXPECT_FALSE(creators.empty());
}

TEST(AlgorithmFactory, ThrowsForUnknownAlgorithm)
{
    EXPECT_THROW(
        static_cast<void>(skeletonizer::algorithm_factory::creators_for(
            "unknown_algorithm",
            skeletonizer::skeletonizer_type::cpu)),
        std::runtime_error);
}

TEST(AlgorithmFactory, CreateReturnsNullForUnknownAlgorithm)
{
    const auto instance = skeletonizer::algorithm_factory::create("unknown_algorithm", skeletonizer::skeletonizer_type::cpu);
    EXPECT_EQ(instance, nullptr);
}
