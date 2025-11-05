module;

#include <cmath>
#include <filesystem>
#include <fstream>
#include <limits>
#include <memory>
#include <ranges>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

#include "benchmark/benchmark.h"
#include "glog/logging.h"

export module benchmark:manager;

import configuration;
import visual_inspector;
import commandline;

import :runner;
import :helpers;
import :aggregator;
import :exporter;

namespace skeletonization_benchmark
{
	export class manager
	{
	public:
		void register_all()
		{
			const auto& arguments = global_arguments();

			for (const auto& image_metadata : arguments.configuration_path.empty()
				                                  ? configuration::load_skeletonizer_configuration(
					                                  arguments.skeletonizer_configuration)
				                                  : configuration::load_skeletonizer_configuration(
					                                  arguments.configuration_path))
			{
				add_runner(image_metadata);
			}
		}

		static std::string run_all()
		{
			std::cout << "Starting Skeletonization Algorithms Benchmarks\n\n" << std::endl;
			std::ostringstream output_json;

			benchmark::JSONReporter json;
			benchmark::ConsoleReporter console;

			json.SetOutputStream(&output_json);

			composite_reporter reporter(console, json);

			benchmark::RunSpecifiedBenchmarks(&reporter);

			std::cout << "\n\nFinished Skeletonization Algorithms Benchmarks\n\n" << std::endl;

			return output_json.str();
		}

		void add_runner(const configuration::image_benchmark_metadata& image_metadata)
		{
			auto benchmark_runner = std::make_unique<runner>(image_metadata);

			benchmark_runner->register_all_benchmarks();

			runners_.emplace_back(image_metadata.name, std::move(benchmark_runner));
		}

		void delete_runner(const std::string& name)
		{
			std::erase_if(runners_, [&](const auto& pair)
			{
				return pair.first == name;
			});
		}

		void show_results(const std::string& benchmark_json) const
		{
			if (benchmark_json.empty())
			{
				LOG(WARNING) << "Empty benchmark JSON provided.";
				return;
			}

			const auto metrics = parse_google_benchmark_output(benchmark_json);

			const auto packages = aggregator::build(runners_, metrics);

			const auto visualizer_path = std::filesystem::path(VISUALIZER_ASSETS_DIR);

			const auto visualizer_index_path = std::filesystem::path(VISUALIZER_ASSETS_DIR) / "index.html";

			const auto visualizer_output_json = visualizer_path / "benchmark_data.json";

			const auto export_result = exporter::write_output_json(packages, visualizer_output_json) &&
				exporter::write_output_json(packages, global_arguments().benchmark_out);

			if (!export_result)
			{
				LOG(ERROR) << "Failed to export visualizer JSON: " << visualizer_output_json;
				return;
			}

			visual_inspector::visualiser::show(visualizer_index_path);
		}

	private:
		std::vector<std::pair<std::string, std::unique_ptr<runner>>> runners_;
	};
}
