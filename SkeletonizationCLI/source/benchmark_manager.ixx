module;

#include <memory>
#include <string>
#include <opencv2/opencv.hpp>
#include <vector>
#include <ranges>

#include "benchmark/benchmark.h"

export module benchmark_manager;

import benchmark_runner;
import skeletonizer_registry;

export class benchmark_manager
{
public:
	void register_all()
	{
		for (const auto &image_metadata : skeletonizer_registry)
		{
			add_runner(image_metadata);
		}
	}

	void run_all() const
	{
		benchmark::RunSpecifiedBenchmarks();
	}

	void add_runner(const image_benchmark_metadata &image_metadata)
	{
		auto runner = std::make_unique<benchmark_runner>(image_metadata);

		runner->register_all_benchmarks();

		runners_.emplace_back(image_metadata.name, std::move(runner));
	}

	void delete_runner(const std::string &name)
	{
		std::erase_if(runners_, [&](const auto &pair)
					  { return pair.first == name; });
	}

	void show_results() const
	{
		std::ranges::for_each(std::views::values(runners_), [](const auto &runner)
							  { runner->show_all_results(); });
	}

private:
	std::vector<std::pair<std::string, std::unique_ptr<benchmark_runner>>> runners_;
};
