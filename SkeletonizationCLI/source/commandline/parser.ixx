module;

#include "CLI/CLI.hpp"
#include "glog/logging.h"

export module commandline:parser;

import :arguments;

namespace commandline
{
	export class parser
	{
	public:
		explicit parser(const int argc, const char* const* argv) : argc_(argc), argv_(argv)
		{
		}

		void parse() const
		{
			auto& args = global_arguments();

			CLI::App app{"Skeletonization benchmark"};

			app.add_option("-c,--config", args.configuration_path, "Path to configuration JSON file")
			   ->check(CLI::ExistingFile)
			   ->default_val(default_configuration_path);

			app.add_option("--benchmark_out", args.benchmark_out, "Path to benchmark file output");

			app.add_option("--benchmark_out_format", args.benchmark_out_format, "Path to benchmark file output");

			auto current_leaf = std::make_shared<skeletonizer_config>();

			std::vector<skeletonizer_config> skeletonizer_configurations;

			const auto finalize_current_leaf = [&]
			{
				if (!current_leaf->name.empty() || !current_leaf->path.empty() || !current_leaf->skeletonizers.empty())
				{
					skeletonizer_configurations.push_back(*current_leaf);
					current_leaf = std::make_shared<skeletonizer_config>();
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
						current_leaf->path = path;
				},
				"Path to the leaf image");

			app.add_option_function<std::vector<std::string>>("--skeleton",
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

					                                                  current_leaf->skeletonizers.push_back({
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

			args.skeletonizer_configuration = std::move(skeletonizer_configurations);

			const bool has_cli_leaves = !args.skeletonizer_configuration.empty();

			const bool has_json_config = !args.configuration_path.empty() && args.configuration_path !=
				default_configuration_path;

			if (has_cli_leaves && has_json_config)
			{
				LOG(FATAL) <<
					"Error: you cannot specify both --config and CLI leaves (--leaf-name, --leaf-path, --skeleton).";
			}

			if (!has_cli_leaves && !has_json_config)
			{
				LOG(FATAL) << "Error: either --config or at least one CLI leaf must be specified.";
			}

			if (has_cli_leaves)
			{
				args.configuration_path.clear();
			}
		}

	private:
		const int argc_;
		const char* const* argv_;
	};
}
