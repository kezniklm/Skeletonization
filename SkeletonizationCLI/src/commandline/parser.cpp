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
#include "SkeletonizationCore/configuration/types.hpp"

namespace
{
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
		catch (...)
		{
			return "invalid path";
		}
	}

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
	parser::parser(const int argc, const char* const* argv) : argc_(argc), argv_(argv)
	{
	}

	void parser::parse() const
	{
		auto& args = global_arguments();

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
						throw std::runtime_error(
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
		catch (const CLI::ParseError& e)
		{
			std::exit(app.exit(e));
		}

		finalize_current_leaf();

		args.skeletonizer_configuration =
			std::move(skeletonizer_configurations);

		const bool has_cli_leaves =
			!args.skeletonizer_configuration.empty();

		const bool has_json_config = !args.configuration_path.empty();

		if (has_cli_leaves && has_json_config)
		{
			LOG(FATAL)
				<< "Error: you cannot specify both --config and CLI leaves "
				"(--leaf-name, --leaf-path, --skeleton).";
		}

		if (!has_cli_leaves && !has_json_config)
		{
			LOG(FATAL)
				<< "Error: either --config or at least one CLI leaf must be "
				"specified.";
		}

		if (has_cli_leaves)
		{
			args.configuration_path.clear();
		}
	}
}
