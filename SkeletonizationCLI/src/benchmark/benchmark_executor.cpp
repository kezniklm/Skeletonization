#include "SkeletonizationCLI/benchmark/benchmark_executor.hpp"

#include <sstream>

#include "benchmark/benchmark.h"
#include "glog/logging.h"

#include "SkeletonizationCLI/benchmark/helpers.hpp"
#include "SkeletonizationCLI/exceptions/benchmark_exception.hpp"

namespace skeletonization_benchmark
{
	std::string benchmark_executor::run(const benchmark_registry& registry) const
	{
		return run(registry, true, true);
	}

	std::string benchmark_executor::run(const benchmark_registry& registry,
	                                    const bool output_json,
	                                    const bool output_console) const
	{
		if (registry.empty())
		{
			throw cli::exceptions::no_benchmarks_registered_exception();
		}

		LOG(INFO) << "Starting Skeletonization Algorithms Benchmarks";

		std::ostringstream output_stream;

		if (output_json && output_console)
		{
			benchmark::JSONReporter json;
			benchmark::ConsoleReporter console;

			json.SetOutputStream(&output_stream);

			composite_reporter reporter(console, json);

			benchmark::RunSpecifiedBenchmarks(&reporter);
		}
		else if (output_json)
		{
			benchmark::JSONReporter json;
			json.SetOutputStream(&output_stream);

			benchmark::RunSpecifiedBenchmarks(&json);
		}
		else if (output_console)
		{
			benchmark::ConsoleReporter console;

			benchmark::RunSpecifiedBenchmarks(&console);
		}
		else
		{
			benchmark::RunSpecifiedBenchmarks();
		}

		LOG(INFO) << "Finished Skeletonization Algorithms Benchmarks";

		return output_stream.str();
	}
}
