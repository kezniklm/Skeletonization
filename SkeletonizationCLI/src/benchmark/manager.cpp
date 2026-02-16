#include <filesystem>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <opencv2/opencv.hpp>

#include "benchmark/benchmark.h"
#include "glog/logging.h"

#include "SkeletonizationCLI/benchmark/helpers.hpp"
#include "SkeletonizationCLI/benchmark/manager.hpp"
#include "SkeletonizationCLI/benchmark/runner.hpp"
#include "SkeletonizationCLI/benchmark/exporter.hpp"
#include "SkeletonizationCLI/benchmark/aggregator.hpp"
#include "SkeletonizationCLI/configuration/loader.hpp"
#include "SkeletonizationCLI/commandline/arguments.hpp"
#include "SkeletonizationCLI/visual_inspector/visualiser.hpp"

namespace skeletonization_benchmark
{
	void manager::register_all()
	{
		const auto& arguments = global_arguments();

		const auto configs = arguments.configuration_path.empty()
								 ? configuration::load_skeletonizer_configuration(
									 arguments.skeletonizer_configuration)
								 : configuration::load_skeletonizer_configuration(
									 arguments.configuration_path);

		configurations_ = configs;

		for (const auto& image_metadata : configs)
		{
			add_runner(image_metadata);
		}
	}

	std::string manager::run_all()
	{
		std::cout << "Starting Skeletonization Algorithms Benchmarks\n\n"
			<< std::endl;

		std::ostringstream output_json;

		benchmark::JSONReporter json;
		benchmark::ConsoleReporter console;

		json.SetOutputStream(&output_json);

		composite_reporter reporter(console, json);

		benchmark::RunSpecifiedBenchmarks(&reporter);

		std::cout << "\n\nFinished Skeletonization Algorithms Benchmarks\n\n"
			<< std::endl;

		return output_json.str();
	}

	void manager::add_runner(const configuration::image_benchmark_metadata& image_metadata)
	{
		auto benchmark_runner = std::make_unique<runner>(image_metadata);
		benchmark_runner->register_all_benchmarks();

		runners_.emplace_back(image_metadata.name, std::move(benchmark_runner));
	}

	void manager::delete_runner(const std::string& name)
	{
		std::erase_if(runners_, [&](const auto& pair)
		{
			return pair.first == name;
		});
	}

	void manager::show_results(const std::string& benchmark_json) const
	{
		if (benchmark_json.empty())
		{
			LOG(WARNING) << "Empty benchmark JSON provided.";
			return;
		}

		const auto metrics = parse_google_benchmark_output(benchmark_json);
		const auto packages = aggregator::build(runners_, metrics);

		const auto visualizer_path = std::filesystem::path(VISUALIZER_ASSETS_DIR);
		const auto visualizer_index_path = visualizer_path / "index.html";
		const auto visualizer_output_json = visualizer_path / "benchmark_data.json";
		const auto visualizer_config_json = visualizer_path / "configuration.json";

		const auto& args = global_arguments();

		const auto timestamped_output_json = args.benchmark_out.empty()
													 ? std::filesystem::path{}
													 : exporter::create_timestamped_output_path("benchmark_data.json");

		const auto timestamped_config_json = args.benchmark_out.empty()
												  ? std::filesystem::path{}
												  : exporter::create_timestamped_output_path("configuration.json");

		const auto export_result =
			exporter::write_output_json(packages, visualizer_output_json) &&
			exporter::write_output_json(packages, timestamped_output_json) &&
			exporter::write_configuration_json(configurations_, visualizer_config_json) &&
			(timestamped_config_json.empty() || exporter::write_configuration_json(configurations_, timestamped_config_json));

		if (!export_result)
		{
			LOG(ERROR) << "Failed to export visualizer JSON: "
				<< visualizer_output_json;
			return;
		}

		visual_inspector::visualiser::show(visualizer_index_path);
	}
}
