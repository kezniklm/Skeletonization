module;

#include <memory>
#include <string>
#include <opencv2/opencv.hpp>
#include <vector>
#include <ranges>

#include "benchmark/benchmark.h"

export module benchmark:manager;

import configuration;
import visual_inspector;

import :runner;

namespace skeletonization_benchmark
{
	export class manager
	{
	public:
		void register_all()
		{
			for (const auto& image_metadata : skeletonizer_configuration)
			{
				add_runner(image_metadata);
			}
		}

		static void run_all()
		{
			std::cout << "Starting Skeletonization Algorithms Benchmarks\n\n" << std::endl;
			benchmark::RunSpecifiedBenchmarks();
			std::cout << "\n\nFinished Skeletonization Algorithms Benchmarks" << std::endl;
		}

		void add_runner(const image_benchmark_metadata& image_metadata)
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

		void show_results() const
		{
			 visual_inspector::visualiser visualiser;

			 std::ranges::for_each(std::views::values(runners_), [&visualiser](const auto& runner)
				 {
					 auto benchmark_image_container = runner->process_benchmark_results();
					 visualiser.add_benchmark_image_container(benchmark_image_container);
				 });

			 constexpr auto default_window_name = "Skeletonization algorithms comparison";

			 visualiser.show(default_window_name);
		}

	private:
		std::vector<std::pair<std::string, std::unique_ptr<runner>>> runners_;
	};
}