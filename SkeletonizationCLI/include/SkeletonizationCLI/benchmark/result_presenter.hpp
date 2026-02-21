#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "aggregator.hpp"
#include "benchmark_registry.hpp"
#include "helpers.hpp"
#include "SkeletonizationCLI/interfaces/i_exporter.hpp"
#include "SkeletonizationCLI/interfaces/i_visualizer.hpp"
#include "SkeletonizationCore/configuration/types.hpp"

namespace skeletonization_benchmark
{
	/**
	 * @brief Presents benchmark results to the user.
	 *
	 * Handles result aggregation, export, and visualization,
	 * following Single Responsibility Principle.
	 */
	class result_presenter
	{
	public:
		result_presenter() = default;

		/**
		 * @brief Construct with injected dependencies.
		 */
		result_presenter(std::shared_ptr<cli::interfaces::i_exporter> exporter,
		                 std::shared_ptr<cli::interfaces::i_visualizer> visualizer)
			: exporter_(std::move(exporter)), visualizer_(std::move(visualizer))
		{
		}

		/**
		 * @brief Set the configurations for result context.
		 */
		void set_configurations(std::vector<configuration::image_benchmark_metadata> configs);

		/**
		 * @brief Present results from benchmark JSON output.
		 * @param registry The registry with runners that generated results.
		 * @param benchmark_json JSON output from benchmark execution.
		 * @param visualizer_path Path to the visualizer assets.
		 * @param output_path Optional path for timestamped output files.
		 */
		void present(const benchmark_registry& registry,
		             const std::string& benchmark_json,
		             const std::filesystem::path& visualizer_path,
		             const std::filesystem::path& output_path = {}) const;

		/**
		 * @brief Export results to files without visualization.
		 * @param registry The registry with runners.
		 * @param benchmark_json JSON output from benchmark execution.
		 * @param output_path Path for output files.
		 * @return true if export succeeded.
		 */
		[[nodiscard]] bool export_results(const benchmark_registry& registry,
		                                  const std::string& benchmark_json,
		                                  const std::filesystem::path& output_path) const;

	private:
		std::vector<configuration::image_benchmark_metadata> configurations_;
		std::shared_ptr<cli::interfaces::i_exporter> exporter_;
		std::shared_ptr<cli::interfaces::i_visualizer> visualizer_;
	};
}
