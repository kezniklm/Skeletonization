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
