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
		 */
		void add_runner(const configuration::image_benchmark_metadata& image_metadata);

		/**
		 * @brief Remove a benchmark runner by name.
		 */
		void delete_runner(const std::string& name);

		/**
		 * @brief Display benchmark results.
		 */
		void show_results(const std::string& benchmark_json) const;

		/**
		 * @brief Get read-only access to the underlying registry.
		 */
		[[nodiscard]] const benchmark_registry& registry() const noexcept
		{
			return registry_;
		}

	private:
		benchmark_registry registry_;
		benchmark_executor executor_;
		mutable result_presenter presenter_;
		std::vector<configuration::image_benchmark_metadata> configurations_;

		std::shared_ptr<cli::interfaces::i_arguments_provider> args_provider_;
		std::shared_ptr<cli::interfaces::i_configuration_loader> config_loader_;
	};
}
