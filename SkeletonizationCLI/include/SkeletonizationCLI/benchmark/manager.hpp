/**
*
* @file manager.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares benchmark management orchestration.
*
* This file defines a high-level benchmark manager facade that coordinates
* configuration loading, registration, execution, and presentation.
*
* Main responsibilities:
* - register benchmarks from loaded configurations
* - execute registered benchmarks
* - present and export benchmark results
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "benchmark_executor.hpp"
#include "benchmark_registry.hpp"
#include "result_presenter.hpp"
#include "runner.hpp"

#include "SkeletonizationCLI/interfaces/i_arguments_provider.hpp"
#include "SkeletonizationCLI/interfaces/i_configuration_loader.hpp"
#include "SkeletonizationCLI/interfaces/i_exporter.hpp"
#include "SkeletonizationCLI/interfaces/i_runner_factory.hpp"
#include "SkeletonizationCLI/interfaces/i_visualizer.hpp"
#include "SkeletonizationCore/configuration/types.hpp"

namespace skeletonization_benchmark
{
	/**
	 * @class manager
	 * @brief High-level facade for benchmark management.
	 *
	 * Provides a simplified interface for the common use case of
	 * registering, running, and presenting benchmarks. Internally
	 * delegates to specialized classes following SRP.
	 *
	 * For more control, use benchmark_registry, benchmark_executor,
	 * and result_presenter directly.
	 */
	class manager
	{
	public:
		/**
		 * @brief Construct manager with dependency injection.
		 * @param args_provider Command-line arguments provider.
		 * @param config_loader Configuration loader service.
		 * @param runner_factory Runner factory service.
		 * @param exporter Export service for benchmark artifacts.
		 * @param visualizer Visualization service.
		 */
		manager(std::shared_ptr<cli::interfaces::i_arguments_provider> args_provider,
		        std::shared_ptr<cli::interfaces::i_configuration_loader> config_loader,
		        std::shared_ptr<cli::interfaces::i_runner_factory> runner_factory,
		        std::shared_ptr<cli::interfaces::i_exporter> exporter,
		        std::shared_ptr<cli::interfaces::i_visualizer> visualizer)
			: registry_(std::move(runner_factory))
			  , presenter_(std::move(exporter), std::move(visualizer))
			  , args_provider_(std::move(args_provider))
			  , config_loader_(std::move(config_loader))
		{
		}

		/**
		 * @brief Register all benchmarks from configuration.
		 */
		void register_all();

		/**
		 * @brief Run all registered benchmarks.
		 * @return JSON string containing benchmark results.
		 */
		[[nodiscard]] std::string run_all() const;

		/**
		 * @brief Add a benchmark runner for the given image metadata.
		 * @param image_metadata Metadata used to create runner.
		 */
		void add_runner(const configuration::image_benchmark_metadata& image_metadata);

		/**
		 * @brief Remove a benchmark runner by name.
		 * @param name Benchmark runner name.
		 */
		void delete_runner(const std::string& name);

		/**
		 * @brief Display benchmark results.
		 * @param benchmark_json JSON payload with benchmark results.
		 */
		void show_results(const std::string& benchmark_json) const;

		/**
		 * @brief Get read-only access to the underlying registry.
		 * @return Const reference to benchmark registry.
		 */
		[[nodiscard]] const benchmark_registry& registry() const noexcept
		{
			return registry_;
		}

	private:
		/// Registry of benchmark runners.
		benchmark_registry registry_;
		/// Benchmark execution service.
		benchmark_executor executor_;
		/// Result presentation service.
		mutable result_presenter presenter_;
		/// Loaded configuration metadata.
		std::vector<configuration::image_benchmark_metadata> configurations_;

		/// Injected arguments provider.
		std::shared_ptr<cli::interfaces::i_arguments_provider> args_provider_;
		/// Injected configuration loader.
		std::shared_ptr<cli::interfaces::i_configuration_loader> config_loader_;
	};
}
