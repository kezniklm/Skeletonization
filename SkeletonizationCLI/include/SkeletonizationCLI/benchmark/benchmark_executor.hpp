#pragma once

#include <string>

#include "benchmark_registry.hpp"

namespace skeletonization_benchmark
{
	/**
	 * @brief Executes registered benchmarks.
	 *
	 * Separates benchmark execution from registration and result presentation,
	 * following Single Responsibility Principle.
	 */
	class benchmark_executor
	{
	public:
		/**
		 * @brief Run all benchmarks in the given registry.
		 * @param registry The registry containing runners to execute.
		 * @return JSON string containing benchmark results.
		 * @throws no_benchmarks_registered_exception if registry is empty.
		 */
		[[nodiscard]] std::string run(const benchmark_registry& registry) const;

		/**
		 * @brief Run benchmarks with custom reporter configuration.
		 * @param registry The registry containing runners to execute.
		 * @param output_json Whether to output JSON format.
		 * @param output_console Whether to output to console.
		 * @return JSON string containing benchmark results (if enabled).
		 */
		[[nodiscard]] std::string run(const benchmark_registry& registry,
		                              bool output_json,
		                              bool output_console) const;
	};
}
