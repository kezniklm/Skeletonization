/**
 *
 * @file cli_configuration_loader_tests.cpp
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Tests the CLI configuration loader.
 *
 * This file contains unit tests for loading benchmark configurations.
 *
 * @version 1.0
 * @date 2026-03-16
 */

#include "gtest/gtest.h"

#include <filesystem>
#include <fstream>
#include <string>

#include "SkeletonizationCLI/configuration/loader.hpp"
#include "SkeletonizationCLI/exceptions/configuration_exception.hpp"

namespace
{
    std::filesystem::path make_temp_file(const std::string& content)
    {
        const auto path = std::filesystem::temp_directory_path() / "skeletonization_cli_config_test.json";
        std::ofstream out(path);
        out << content;
        return path;
    }
}

TEST(CliConfigurationLoader, ThrowsWhenFileMissing)
{
    configuration::configuration_loader loader;
    EXPECT_THROW(static_cast<void>(loader.load("missing_config.json")),
        cli::exceptions::configuration_file_not_found_exception);
}

TEST(CliConfigurationLoader, ThrowsOnInvalidJson)
{
    configuration::configuration_loader loader;
    const auto path = make_temp_file("{ invalid json }");

    EXPECT_THROW(static_cast<void>(loader.load(path.string())),
        cli::exceptions::configuration_parse_exception);
}

TEST(CliConfigurationLoader, ThrowsOnEmptyArray)
{
    configuration::configuration_loader loader;
    const auto path = make_temp_file("[]");

    EXPECT_THROW(static_cast<void>(loader.load(path.string())),
        cli::exceptions::configuration_validation_exception);
}

TEST(CliConfigurationLoader, LoadsValidArray)
{
    configuration::configuration_loader loader;
    const auto path = make_temp_file(R"([
        {
            "name": "Leaf A",
            "path": "leaf_a.png",
            "skeletonizers": [
                { "type": "cpu", "algorithm": "zhang_suen" }
            ]
        }
    ])");

    const auto entries = loader.load(path.string());
    ASSERT_EQ(entries.size(), 1u);
    EXPECT_EQ(entries.front().name, "Leaf A");
    EXPECT_EQ(entries.front().path, "leaf_a.png");
    EXPECT_FALSE(entries.front().skeletonizers.empty());
}

TEST(CliConfigurationLoader, SkipsInvalidEntries)
{
    configuration::configuration_loader loader;
    const auto path = make_temp_file(R"([
        {
            "name": "Leaf A",
            "path": "leaf_a.png",
            "skeletonizers": [
                { "type": "cpu", "algorithm": "zhang_suen" }
            ]
        },
        {
            "name": "Leaf B",
            "path": "leaf_b.png",
            "skeletonizers": [
                { "type": "unknown", "algorithm": "zhang_suen" }
            ]
        }
    ])");

    const auto entries = loader.load(path.string());

    ASSERT_EQ(entries.size(), 1u);
    EXPECT_EQ(entries.front().name, "Leaf A");
}

TEST(CliConfigurationLoader, LoadsFromInMemoryConfig)
{
    configuration::configuration_loader loader;
    configuration::skeletonizer_config config;
    config.name = "Leaf B";
    config.path = "leaf_b.png";
    config.skeletonizers.push_back({"cpu", "zhang_suen"});

    const auto entries = loader.load(std::vector<configuration::skeletonizer_config>{config});
    ASSERT_EQ(entries.size(), 1u);
    EXPECT_EQ(entries.front().name, "Leaf B");
    EXPECT_EQ(entries.front().path, "leaf_b.png");
    EXPECT_FALSE(entries.front().skeletonizers.empty());
}
