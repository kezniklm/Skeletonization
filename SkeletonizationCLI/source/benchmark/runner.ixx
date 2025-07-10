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
				auto skeletonizer = skeletonizer_creator();
				const std::string name = create_benchmark_name(skeletonizer->name(), type);

				benchmark::RegisterBenchmark(name,
				                             [this, skeletonizer = std::move(skeletonizer), name](
				                             benchmark::State& state)
				                             {
					                             for (auto _ : state)
					                             {
						                             const auto result = skeletonizer->apply(binary_image_);
						                             results_[name] = result;
					                             }
				                             });
			}
		}

		std::string create_benchmark_name(const std::string& skeletonizer_name, const skeletonizer_type type) const
		{
			return image_metadata_.name + "/" + skeletonizer_name + "/" + to_string(type);
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
