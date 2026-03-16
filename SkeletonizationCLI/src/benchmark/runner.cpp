/**
*
* @file runner.cpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Implements benchmark run execution pipelines.
*
* This file implements benchmark runner logic for one input image.
*
* Main responsibilities:
* - load and preprocess input image data
* - register and execute algorithm benchmarks
* - collect and package output artifacts
*
* @version 1.0
* @date 2026-03-16
*/

#include "SkeletonizationCLI/benchmark/runner.hpp"

#include <functional>
#include <string>
#include <utility>

#include <benchmark/benchmark.h>
#include <opencv2/opencv.hpp>

#if SKELETONIZATION_WITH_GPU
#include <opencv2/core/cuda_stream_accessor.hpp>
#endif

#include "glog/logging.h"
#include "SkeletonizationCLI/visual_inspector/image_container.hpp"
#include "SkeletonizationCore/configuration/types.hpp"
#include "SkeletonizationCore/extensions/image_processing.hpp"
#include "SkeletonizationCore/skeletonizer/skeletonizer.hpp"


namespace skeletonization_benchmark
{
	runner::runner(const configuration::image_benchmark_metadata& image_metadata,
	               std::shared_ptr<cli::interfaces::i_arguments_provider> args_provider,
	               const cli::interfaces::i_image_loader& image_loader,
	               const cli::interfaces::i_image_preprocessor& image_preprocessor)
		: image_metadata_(image_metadata)
		  , args_provider_(std::move(args_provider))
		  , input_image_(image_loader.load(image_metadata.path))
		  , binary_image_(args_provider_->run_image_preprocessing()
			                  ? image_preprocessor.preprocess(input_image_)
			                  : input_image_)
	{
	}

	void runner::register_all_benchmarks()
	{
		for (const auto& [type, creators] : image_metadata_.skeletonizers)
		{
			for (const auto& creator : creators)
			{
				auto skeletonizer_instance = creator();

				const auto name =
					create_benchmark_name(skeletonizer_instance->name(), type);

				register_benchmark(name, std::move(skeletonizer_instance));
			}
		}
	}

	visual_inspector::image_container runner::process_benchmark_results() const
	{
		visual_inspector::image_container image_container(image_metadata_.name);

		constexpr auto default_input_image_name = "Original Image";

		image_container.add_image(input_image_, default_input_image_name);

		for (const auto& [name, image] : results_)
		{
			if (name.empty())
			{
				image_container.add_image(image);
			}
			else
			{
				image_container.add_image(image, name);
			}
		}

		return image_container;
	}

	void runner::register_benchmark(const std::string& name,
	                                std::unique_ptr<skeletonizer::skeletonizer<>> skeletonizer_instance)
	{
		if (name.size() > max_total_algorithm_name_length)
		{
			LOG(WARNING) << "Skipping benchmark for \"" << name
				<< "\" (name too long, limit is "
				<< user_algorithm_name_length << " characters)"
				<< std::endl << std::endl;
			return;
		}

#if SKELETONIZATION_WITH_GPU
		const bool is_gpu_benchmark = name.find("/GPU") != std::string::npos;
#else
		constexpr bool is_gpu_benchmark = false;
#endif

		benchmark::RegisterBenchmark(
			name,
			[this,
				name,
				is_gpu_benchmark,
				skeletonizer = std::move(skeletonizer_instance)](benchmark::State& state) mutable
			{
#if SKELETONIZATION_WITH_GPU

				if (is_gpu_benchmark && state.thread_index() == 0)
				{
					constexpr int warmup_iterations = 3;

					for (int warmup_iteration = 0; warmup_iteration < warmup_iterations; ++warmup_iteration)
					{
						auto warmup_image = binary_image_.clone();

						skeletonizer->apply(warmup_image);
					}

					cv::cuda::Stream::Null().waitForCompletion();
				}
#endif

				for (auto _ : state)
				{
					try
					{
						auto image = binary_image_.clone();

						skeletonizer->apply(image);

						results_[name] = scale(image);
					}
					catch (const std::exception& e)
					{
						LOG(ERROR) << "Skeletonizer '" << name << "' failed: " << e.what();
						state.SkipWithError(e.what());
						break;
					}
				}
			})->Iterations(args_provider_->benchmark_iterations());
	}

	std::string runner::create_benchmark_name(const std::string& skeletonizer_name,
	                                          const skeletonizer::skeletonizer_type type) const
	{
		return image_metadata_.name + "/" + skeletonizer_name + "/" + to_string(type);
	}
}
