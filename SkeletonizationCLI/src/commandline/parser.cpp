/**
*
* @file parser.cpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Implements command-line parsing behavior.
*
* This file implements command-line parsing and validation logic.
*
* Main responsibilities:
* - parse raw process arguments
* - validate CLI option values
* - build typed argument structures
*
* @version 1.0
* @date 2026-03-16
*/

#include "SkeletonizationCLI/commandline/parser.hpp"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "CLI/CLI.hpp"
#include "glog/logging.h"
#include "SkeletonizationCLI/commandline/arguments.hpp"
#include "SkeletonizationCLI/exceptions/configuration_exception.hpp"
#include "SkeletonizationCore/configuration/types.hpp"

namespace
{
	/**
	 * @brief Validates JSON file extension for output path argument.
	 *
	 * @param s Candidate output path.
	 * @return Empty string when valid, otherwise validation error text.
	 */
	std::string json_path_check(const std::string& s)
	{
		if (s.empty())
		{
			return {};
		}
		try
		{
			auto ext = std::filesystem::path(s).extension().string();

			for (auto& c : ext)
			{
				c = static_cast<char>(
					std::tolower(static_cast<unsigned char>(c)));
			}

			return ext == ".json" ? std::string{} : std::string{"must end with .json"};
		}
		catch (const std::exception& e)
		{
			return "invalid path: " + std::string(e.what());
		}
	}

	/**
	 * @brief Ensures parent directory of a path exists.
	 *
	 * @param p Target file path.
	 */
	void ensure_parent_exists(const std::string& p)
	{
		std::filesystem::path path{p};

		if (path.has_parent_path())
		{
			std::filesystem::create_directories(path.parent_path());
		}
	}
}

namespace commandline
{
	/**
	 * @brief Constructs parser with raw argc/argv arguments.
	 *
	 * @param argc Argument count.
	 * @param argv Argument vector.
	 */
	parser::parser(const int argc, const char* const* argv) : argc_(argc), argv_(argv)
	{
	}

	/**
	 * @brief Parses command line and returns typed benchmark arguments.
	 *
	 * @return Parsed command line arguments.
	 */
	arguments parser::parse() const
	{
		arguments args;

		CLI::App app{"Skeletonization benchmark"};

		app.add_option(
			   "-c,--config",
			   args.configuration_path,
			   "Path to configuration JSON file")
		   ->check(CLI::ExistingFile)
		   ->default_val(default_configuration_path);

		app.add_option(
			   "--benchmark_out",
			   args.benchmark_out,
			   "Path to benchmark file output")
		   ->default_val("./benchmark.json");

		app.add_option(
			   "--run_image_preprocessing",
			   args.run_image_preprocessing,
			   "Enables image preprocessing")
		   ->default_val(true);

		app.callback([&]
		{
			if (args.benchmark_out.empty())
			{
				throw CLI::ValidationError(
					"--benchmark_out",
					"Path to benchmark file output cant be empty");
			}

			const auto msg = json_path_check(args.benchmark_out);

			if (!msg.empty())
			{
				throw CLI::ValidationError("--benchmark_out", msg);
			}

			ensure_parent_exists(args.benchmark_out);
		});

		auto current_leaf =
			std::make_shared<configuration::skeletonizer_config>();

		std::vector<configuration::skeletonizer_config> skeletonizer_configurations;

		const auto finalize_current_leaf = [&]
		{
			if (!current_leaf->name.empty()
				|| !current_leaf->path.empty()
				|| !current_leaf->skeletonizers.empty())
			{
				skeletonizer_configurations.push_back(*current_leaf);
				current_leaf =
					std::make_shared<configuration::skeletonizer_config>();
			}
		};

		app.add_option_function<std::vector<std::string>>(
			"--leaf-name",
			[&](const std::vector<std::string>& names)
			{
				for (const auto& name : names)
				{
					finalize_current_leaf();
					current_leaf->name = name;
				}
			},
			"Name of the leaf");

		app.add_option_function<std::vector<std::string>>(
			"--leaf-path",
			[&](const std::vector<std::string>& paths)
			{
				for (const auto& path : paths)
				{
					current_leaf->path = path;
				}
			},
			"Path to the leaf image");

		app.add_option_function<std::vector<std::string>>(
			"--skeleton",
			[&](const std::vector<std::string>& skeletons)
			{
				for (const auto& skeleton : skeletons)
				{
					const auto position = skeleton.find(':');

					if (position == std::string::npos)
					{
						throw cli::exceptions::configuration_validation_exception(
							"--skeleton must be type:algorithm, e.g., cpu:zhang_suen");
					}

					current_leaf->skeletonizers.push_back(
						{
							skeleton.substr(0, position),
							skeleton.substr(position + 1)
						});
				}
			},
			"Skeletonizer in format type:algorithm (e.g., cpu:zhang_suen)");

		try
		{
			app.parse(argc_, argv_);
		}
		catch (const CLI::CallForHelp& e)
		{
			const int exit_code = app.exit(e);
			std::exit(exit_code);
		}
		catch (const CLI::CallForAllHelp& e)
		{
			const int exit_code = app.exit(e);
			std::exit(exit_code);
		}
		catch (const CLI::CallForVersion& e)
		{
			const int exit_code = app.exit(e);
			std::exit(exit_code);
		}
		catch (const CLI::ParseError& e)
		{
			throw cli::exceptions::configuration_validation_exception(
				"Command-line parse error: " + std::string(e.what()));
		}

		finalize_current_leaf();

		args.skeletonizer_configurations =
			std::move(skeletonizer_configurations);

		const bool has_cli_leaves =
			!args.skeletonizer_configurations.empty();

		const bool has_json_config = !args.configuration_path.empty();

		if (has_cli_leaves && has_json_config)
		{
			throw cli::exceptions::configuration_validation_exception(
				"Cannot specify both --config and CLI leaves "
				"(--leaf-name, --leaf-path, --skeleton)");
		}

		if (!has_cli_leaves && !has_json_config)
		{
			throw cli::exceptions::configuration_validation_exception(
				"Either --config or at least one CLI leaf must be specified");
		}

		if (has_cli_leaves)
		{
			args.configuration_path.clear();
		}

		return args;
	}
}
