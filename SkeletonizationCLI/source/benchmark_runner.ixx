module;

#include <benchmark/benchmark.h>
#include <opencv2/opencv.hpp>
#include <map>
#include <string>
#include <functional>

export module benchmark_runner;

import skeletonizer;
import skeletonizer_registry;
import image_processing;
import skeletonizer_registry;
import image_viewer;

export class benchmark_runner
{
public:
	explicit benchmark_runner(image_benchmark_metadata image_metadata)
		: image_metadata_(image_metadata),
		  input_image_(cv::imread(image_metadata.path, cv::IMREAD_GRAYSCALE)),
		  binary_image_(binarize(input_image_, 127))
	{
		if (input_image_.empty())
		{
			throw std::runtime_error("Failed to load image: " + image_metadata.path);
		}

		binary_image_ = binarize(input_image_, 127);
	}

	void register_all_benchmarks()
	{
		for (const auto &[type, skeletonizer_creator] : image_metadata_.skeletonizers)
		{
			std::string benchmark_name = image_metadata_.name + "/" + to_string(type);

			benchmark::RegisterBenchmark(benchmark_name,
			                             [this, skeletonizer_creator, benchmark_name](benchmark::State &state)
			                             {
				                             const auto skeletonizer = skeletonizer_creator();

				                             for (auto _ : state)
				                             {
					                             const cv::Mat result = skeletonizer->apply(binary_image_);

					                             results_[benchmark_name] = result;
				                             }
				                             state.SetLabel(skeletonizer->name());
			                             });
		}
	}

	void show_all_results() const
	{
		image_viewer image_viewer(image_metadata_.name);

		constexpr auto default_input_image_name = "Original image";

		image_viewer.add_image(input_image_, default_input_image_name);

		for (const auto &[name, image] : results_)
		{
			if (name.empty())
			{
				image_viewer.add_image(image);
				continue;
			}

			image_viewer.add_image(image, name);
		}

		image_viewer.show();
	}

private:
	image_benchmark_metadata image_metadata_;
	cv::Mat input_image_;
	cv::Mat binary_image_;
	std::map<std::string, cv::Mat> results_;
};
