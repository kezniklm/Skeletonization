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
import commandline;

import :runner;

namespace skeletonization_benchmark
{
	export class manager
	{
	public:
		static void initialize_google_benchmark(const int argc, const char* const* argv)
		{
			std::vector<std::string> args(argv, argv + argc);

			const auto has_user_output_flag = [](const std::vector<std::string>& arguments)
			{
				for (const auto& argument : arguments)
				{
					if (argument.find("--benchmark_out=") == 0)
					{
						return true;
					}
				}

				return false;
			};

			if (!has_user_output_flag(args))
			{
				const auto default_benchmark_results_filename = []
				{
					auto t = std::time(nullptr);

					std::tm tm{};
#ifdef _WIN32
					localtime_s(&tm, &t);
#else
					localtime_r(&t, &tm);
#endif
					std::ostringstream oss;
					oss << "skeletonizers_benchmark_results_"
						<< std::put_time(&tm, "%Y-%m-%d_%H-%M-%S")
						<< ".json";

					return oss.str();
				};

				args.push_back("--benchmark_out=" + default_benchmark_results_filename());
				args.push_back("--benchmark_out_format=json");
			}

			std::vector<char*> mutable_argv;

			mutable_argv.reserve(args.size());

			for (auto& s : args)
			{
				mutable_argv.push_back(const_cast<char*>(s.c_str()));
			}

			int mutable_argc = static_cast<int>(mutable_argv.size());

			benchmark::Initialize(&mutable_argc, mutable_argv.data());
		}

		manager(const int argc, const char* const* argv)
		{
			initialize_google_benchmark(argc, argv);
		}

		void register_all()
		{
			const auto& arguments = global_arguments();

			for (const auto& image_metadata : arguments.configuration_path.empty()
				                                  ? configuration::load_skeletonizer_configuration(
					                                  arguments.skeletonizer_configuration)
				                                  : configuration::load_skeletonizer_configuration(
					                                  arguments.configuration_path))
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
