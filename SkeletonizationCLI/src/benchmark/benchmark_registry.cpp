/**
*
* @file benchmark_registry.cpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Implements benchmark registration lookups.
*
* This file implements registration management for benchmark runners.
*
* Main responsibilities:
* - add and remove benchmark runners
* - maintain runner registry container
* - validate benchmark registration operations
*
* @version 1.0
* @date 2026-03-16
*/

#include "SkeletonizationCLI/benchmark/benchmark_registry.hpp"

#include <algorithm>
#include <stdexcept>

namespace skeletonization_benchmark
{
	void benchmark_registry::add(const configuration::image_benchmark_metadata& image_metadata)
	{
		if (!runner_factory_)
		{
			throw std::logic_error(
				"benchmark_registry requires a runner factory to be set via dependency injection");
		}

		auto benchmark_runner = runner_factory_->create(image_metadata);
		benchmark_runner->register_all_benchmarks();

		runners_.emplace_back(image_metadata.name, std::move(benchmark_runner));
	}

	void benchmark_registry::remove(const std::string& name)
	{
		std::erase_if(runners_, [&](const auto& pair)
		{
			return pair.first == name;
		});
	}

	void benchmark_registry::clear()
	{
		runners_.clear();
	}
}
