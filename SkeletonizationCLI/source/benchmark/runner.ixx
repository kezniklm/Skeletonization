module;

#include <functional>
#include <map>
#include <string>
#include <vector>
#include <benchmark/benchmark.h>
#include <opencv2/opencv.hpp>

#include "glog/logging.h"

export module benchmark:runner;

import skeletonizer;
import configuration;
import image_processing;
import visual_inspector;
import commandline;
import skeletonizer_cpu;

namespace skeletonization_benchmark
{
	export class runner
	{
	public:
		explicit runner(const image_benchmark_metadata& image_metadata)
			: image_metadata_(image_metadata),
			  input_image_(read_image(image_metadata.path)),
			  binary_image_(preprocess_image(input_image_))
		{
		}

		void register_all_benchmarks()
		{
			for (const auto& [type, creators] : image_metadata_.skeletonizers)
			{
				for (const auto& creator : creators)
				{
					auto skeletonizer = creator();

					const std::string name = create_benchmark_name(skeletonizer->name(), type);

					register_benchmark(name, std::move(skeletonizer));
				}
			}
		}

		visual_inspector::image_container process_benchmark_results() const
		{
			visual_inspector::image_container image_container(image_metadata_.name);

			constexpr auto default_input_image_name = "Original Image";

			image_container.add_image(input_image_, default_input_image_name);

			for (const auto& [name, image] : results_)
			{
				if (name.empty())
				{
					image_container.add_image(image);
					continue;
				}

				image_container.add_image(image, name);
			}

			return image_container;
		}

	private:
		void register_benchmark(const std::string& name, std::unique_ptr<skeletonizer::skeletonizer> skeletonizer)
		{
			const auto& arguments = global_arguments();

			if (name.size() > max_total_algorithm_name_length)
			{
				LOG(WARNING) << "Skipping benchmark for \"" << name << "\" (name too long, limit is " <<
					user_algorithm_name_length << " characters)" << std::endl << std::endl;

				return;
			}

			benchmark::RegisterBenchmark(name,
			                             [this, name, skeletonizer = std::move(skeletonizer)](benchmark::State& state)
			                             {
				                             for (auto _ : state)
				                             {
					                             auto image = binary_image_.clone();

					                             skeletonizer->apply(image);

					                             results_[name] = scale(image);
				                             }
			                             })->Iterations(arguments.number_of_benchmark_iterations);
		}

		std::string create_benchmark_name(const std::string& skeletonizer_name,
		                                  const skeletonizer::skeletonizer_type type) const
		{
			return image_metadata_.name + "/" + skeletonizer_name + "/" + to_string(type);
		}

		static constexpr int max_algorithm_type_length = 5;

		static constexpr int max_algorithm_author_length = 20;

		static constexpr int user_algorithm_name_length = 25;

		static constexpr int max_total_algorithm_name_length = user_algorithm_name_length + max_algorithm_author_length
			+ max_algorithm_type_length;

		image_benchmark_metadata image_metadata_;
		cv::Mat input_image_;
		cv::Mat binary_image_;
		std::map<std::string, cv::Mat> results_;
	};
}
