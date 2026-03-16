/**
*
* @file benchmark_executor.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares benchmark execution orchestration types.
*
* This file defines the benchmark execution service that runs registered
* benchmarks and returns serialized result output.
*
* Main responsibilities:
* - execute all registered benchmark runners
* - configure output reporters for runs
* - return serialized benchmark result data
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <string>

#include "benchmark_registry.hpp"

namespace skeletonization_benchmark
{
	/**
	 * @class benchmark_executor
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
