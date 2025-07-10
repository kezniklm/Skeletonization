module;

#include <benchmark/benchmark.h>
#include <opencv2/opencv.hpp>
#include <map>
#include <string>
#include <functional>

export module benchmark:runner;

import skeletonizer;
import configuration;
import image_processing;
import visual_inspector;

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
			for (const auto& [type, skeletonizer_creator] : image_metadata_.skeletonizers)
			{
				std::string benchmark_name = image_metadata_.name + "/" + to_string(type);

				benchmark::RegisterBenchmark(benchmark_name,
				                             [this, skeletonizer_creator, benchmark_name](benchmark::State& state)
				                             {
					                             const auto skeletonizer = skeletonizer_creator();

					                             for (auto _ : state)
					                             {
						                             const auto result = skeletonizer->apply(binary_image_);

						                             results_[benchmark_name] = result;
					                             }
					                             state.SetLabel(skeletonizer->name());
				                             });
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
		image_benchmark_metadata image_metadata_;
		cv::Mat input_image_;
		cv::Mat binary_image_;
		std::map<std::string, cv::Mat> results_;
	};
}
