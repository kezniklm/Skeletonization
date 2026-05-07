/**
 *
 * @file cli_commandline_parser_tests.cpp
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Tests the CLI command-line parser.
 *
 * This file contains unit tests for CLI argument parsing behavior.
 *
 * @version 1.0
 * @date 2026-03-16
 */

#include "gtest/gtest.h"

#include <filesystem>
#include <fstream>
#include <vector>

#include "SkeletonizationCLI/commandline/parser.hpp"
#include "SkeletonizationCLI/exceptions/configuration_exception.hpp"

namespace
{
    std::vector<const char*> make_args(const std::vector<std::string>& parts)
    {
        std::vector<const char*> argv;
        argv.reserve(parts.size());
        for (const auto& part : parts)
        {
            argv.push_back(part.c_str());
        }
        return argv;
    }

    std::filesystem::path make_temp_config()
    {
        const auto path = std::filesystem::temp_directory_path() / "skeletonization_cli_parser_config.json";
        std::ofstream out(path);
        out << "[]";
        return path;
    }
}

TEST(CliCommandlineParser, ParsesConfigPath)
{
   const auto config_path = make_temp_config();
    const std::vector<std::string> parts{
        "skel-cli",
        "--config",
     config_path.string(),
        "--benchmark_out",
        "output.json"
    };
    const auto argv = make_args(parts);

    const commandline::parser parser(static_cast<int>(argv.size()), argv.data());
    const auto args = parser.parse();

 EXPECT_EQ(args.configuration_path, config_path.string());
    EXPECT_EQ(args.benchmark_out, "output.json");
}

TEST(CliCommandlineParser, RejectsMissingConfigAndLeaves)
{
    const std::vector<std::string> parts{
        "skel-cli",
        "--config",
        ""
    };
    const auto argv = make_args(parts);

    const commandline::parser parser(static_cast<int>(argv.size()), argv.data());
    EXPECT_THROW(static_cast<void>(parser.parse()), cli::exceptions::configuration_validation_exception);
}

TEST(CliCommandlineParser, RejectsMissingConfigArgument)
{
    const std::vector<std::string> parts{
        "skel-cli",
        "--config"
    };
    const auto argv = make_args(parts);

    const commandline::parser parser(static_cast<int>(argv.size()), argv.data());
    EXPECT_THROW(static_cast<void>(parser.parse()), cli::exceptions::configuration_validation_exception);
}

TEST(CliCommandlineParser, RejectsConfigAndLeafMix)
{
    const std::vector<std::string> parts{
        "skel-cli",
        "--config",
        "skeletonizer_config.json",
        "--leaf-name",
        "Leaf",
        "--leaf-path",
        "leaf.png",
        "--skeleton",
        "cpu:zhang_suen"
    };
    const auto argv = make_args(parts);

    const commandline::parser parser(static_cast<int>(argv.size()), argv.data());
    EXPECT_THROW(static_cast<void>(parser.parse()), cli::exceptions::configuration_validation_exception);
}

TEST(CliCommandlineParser, ParsesLeafConfiguration)
{
    const std::vector<std::string> parts{
        "skel-cli",
        "--leaf-name",
        "Leaf",
        "--leaf-path",
        "leaf.png",
        "--skeleton",
        "cpu:zhang_suen",
        "--benchmark_out",
        "benchmark.json"
    };
    const auto argv = make_args(parts);

    const commandline::parser parser(static_cast<int>(argv.size()), argv.data());
    EXPECT_THROW(static_cast<void>(parser.parse()), cli::exceptions::configuration_validation_exception);
}

TEST(CliCommandlineParser, RejectsInvalidSkeletonFormat)
{
    const std::vector<std::string> parts{
        "skel-cli",
        "--leaf-name",
        "Leaf",
        "--leaf-path",
        "leaf.png",
        "--skeleton",
        "invalid-format"
    };
    const auto argv = make_args(parts);

    const commandline::parser parser(static_cast<int>(argv.size()), argv.data());
    EXPECT_THROW(static_cast<void>(parser.parse()), cli::exceptions::configuration_validation_exception);
}

TEST(CliCommandlineParser, RejectsInvalidBenchmarkOutputExtension)
{
    const auto config_path = make_temp_config();
    const std::vector<std::string> parts{
        "skel-cli",
        "--config",
        config_path.string(),
        "--benchmark_out",
        "output.txt"
    };
    const auto argv = make_args(parts);

    const commandline::parser parser(static_cast<int>(argv.size()), argv.data());
    EXPECT_THROW(static_cast<void>(parser.parse()), cli::exceptions::configuration_validation_exception);
}
