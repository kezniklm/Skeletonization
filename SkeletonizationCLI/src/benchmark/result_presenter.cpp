/**
*
* @file result_presenter.cpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Implements benchmark result presentation helpers.
*
* This file implements result presentation, export, and visualization calls.
*
* Main responsibilities:
* - transform benchmark output for presentation
* - export result and configuration artifacts
* - trigger visualization rendering workflows
*
* @version 1.0
* @date 2026-03-16
*/

#include "SkeletonizationCLI/benchmark/result_presenter.hpp"

#include <filesystem>
#include <stdexcept>

#include "glog/logging.h"

#include "SkeletonizationCLI/benchmark/exporter.hpp"
#include "SkeletonizationCLI/visual_inspector/visualiser.hpp"

namespace skeletonization_benchmark
{
	void result_presenter::set_configurations(
		std::vector<configuration::image_benchmark_metadata> configs)
	{
		configurations_ = std::move(configs);
	}

	void result_presenter::present(const benchmark_registry& registry,
	                               const std::string& benchmark_json,
	                               const std::filesystem::path& visualizer_path,
	                               const std::filesystem::path& output_path) const
	{
		if (benchmark_json.empty())
		{
			LOG(WARNING) << "Empty benchmark JSON provided.";
			return;
		}

		if (!exporter_)
		{
			throw std::logic_error(
				"result_presenter requires an exporter to be set via dependency injection");
		}

		const auto metrics = parse_google_benchmark_output(benchmark_json);
		const auto packages = aggregator::build(registry.runners(), metrics);

		const auto visualizer_index_path = visualizer_path / "index.html";
		const auto visualizer_output_json = visualizer_path / "benchmark_data.json";
		const auto visualizer_config_json = visualizer_path / "configuration.json";

		const auto timestamped_output_json = output_path.empty()
			                                     ? std::filesystem::path{}
			                                     : exporter::create_timestamped_output_path("benchmark_data.json");

		const auto timestamped_config_json = output_path.empty()
			                                     ? std::filesystem::path{}
			                                     : exporter::create_timestamped_output_path("configuration.json");

		const auto export_result =
			exporter_->export_results(packages, visualizer_output_json) &&
			(timestamped_output_json.empty() || exporter_->export_results(packages, timestamped_output_json)) &&
			exporter_->export_configuration(configurations_, visualizer_config_json) &&
			(timestamped_config_json.empty() || exporter_->export_configuration(
				configurations_, timestamped_config_json));

		if (!export_result)
		{
			LOG(ERROR) << "Failed to export visualizer JSON: "
				<< visualizer_output_json;
			return;
		}

		if (!visualizer_)
		{
			throw std::logic_error(
				"result_presenter requires a visualizer to be set via dependency injection");
		}

		visualizer_->show(visualizer_index_path);
	}

	bool result_presenter::export_results(const benchmark_registry& registry,
	                                      const std::string& benchmark_json,
	                                      const std::filesystem::path& output_path) const
	{
		if (benchmark_json.empty())
		{
			LOG(WARNING) << "Empty benchmark JSON provided.";
			return false;
		}

		if (!exporter_)
		{
			throw std::logic_error(
				"result_presenter requires an exporter to be set via dependency injection");
		}

		const auto metrics = parse_google_benchmark_output(benchmark_json);
		const auto packages = aggregator::build(registry.runners(), metrics);

		const auto output_json = output_path / "benchmark_data.json";
		const auto config_json = output_path / "configuration.json";

		return exporter_->export_results(packages, output_json) &&
			exporter_->export_configuration(configurations_, config_json);
	}
}
