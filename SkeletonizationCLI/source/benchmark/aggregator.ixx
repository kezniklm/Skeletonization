module;

#include <algorithm>
#include <cmath>
#include <limits>
#include <map>
#include <memory>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

export module benchmark:aggregator;

import visual_inspector;
import :runner;
import :helpers;

namespace skeletonization_benchmark
{
	export class aggregator final
	{
	public:
		struct package
		{
			visual_inspector::image_container container;
			std::vector<visual_inspector::image_metrics> metrics_list;
			std::string description;

			double avg_time_ms = 0.0;
			double min_time_ms = 0.0;
			double max_time_ms = 0.0;
			int64_t total_iterations = 0;
			int64_t avg_iterations = 0;
			double std_dev_ms = 0.0;
			double throughput_alg_per_s = 0.0;
			std::string note;
		};

		static std::vector<package> build(const std::vector<std::pair<std::string, std::unique_ptr<runner>>>& runners,
		                                  const metrics_map& metrics_map)
		{
			image_algorithm_record_map per_image;
			original_images_map originals;

			collect_from_runners(runners, metrics_map, per_image, originals);

			std::vector<package> packages;

			for (auto& [image_name, algorithm_records] : per_image)
			{
				packages.push_back(make_package(image_name, algorithm_records, originals));
			}

			return packages;
		}

	private:
		static constexpr auto original_label = "Original";

		struct algorithm_record
		{
			std::string label;
			cv::Mat image;
			visual_inspector::image_metrics metrics{};
		};

		using image_name = std::string;
		using image_algorithm_record_map = std::map<image_name, std::vector<algorithm_record>>;
		using original_images_map = std::map<image_name, cv::Mat>;

		struct summary
		{
			double avg_ms = 0.0;
			double min_ms = 0.0;
			double max_ms = 0.0;
			int64_t total_iters = 0;
			int64_t avg_iters = 0;
			double stddev_ms = 0.0;
			double throughput_per_s = 0.0;
		};


		static void collect_from_runners(const std::vector<std::pair<std::string, std::unique_ptr<runner>>>& runners,
		                                 const metrics_map& metrics_map,
		                                 image_algorithm_record_map& image_algorithm_record_map,
		                                 original_images_map& original_images_map)
		{
			for (const auto& runner : runners | std::views::values)
			{
				const auto image_container = runner->process_benchmark_results();

				const auto image_name = image_container.name();

				if (image_container.size() > 0)
				{
					original_images_map[image_name] = image_container.image(0);
				}

				constexpr auto skip_original_index = 1;

				for (auto i = skip_original_index; i < image_container.size(); ++i)
				{
					const auto full_key = image_container.label(i); // "Image/Alg/Type"

					const auto display = extract_after_first_slash(full_key);

					auto image = image_container.image(i);

					visual_inspector::image_metrics image_metrics{};

					if (auto it = metrics_map.find(full_key); it != metrics_map.end())
					{
						image_metrics = it->second;
					}

					image_algorithm_record_map[image_name].push_back(algorithm_record{
						display, std::move(image), image_metrics
					});
				}
			}
		}

		static package make_package(const std::string& image_name,
		                            std::vector<algorithm_record>& algorithm_records,
		                            const original_images_map& originals)
		{
			visual_inspector::image_container image_container(image_name);

			std::vector<visual_inspector::image_metrics> image_metrics_list;

			if (const auto it = originals.find(image_name); it != originals.end())
			{
				image_container.add_image(it->second, original_label);
				image_metrics_list.emplace_back(); // empty metrics for Original
			}

			std::ranges::sort(algorithm_records,
			                  [](const algorithm_record& first_algorithm_record,
			                     const algorithm_record& second_algorithm_record)
			                  {
				                  return first_algorithm_record.label < second_algorithm_record.label;
			                  });

			for (const auto& [label, image, metrics] : algorithm_records)
			{
				image_container.add_image(image, label);

				image_metrics_list.push_back(metrics);
			}

			const auto [avg_ms,
				min_ms,
				max_ms,
				total_iters,
				avg_iters,
				stddev_ms,
				throughput_per_s] = compute_summary(image_metrics_list);

			return package{
				.container = std::move(image_container),
				.metrics_list = std::move(image_metrics_list),
				.description = "Algorithms tested on " + image_name,
				.avg_time_ms = avg_ms,
				.min_time_ms = min_ms,
				.max_time_ms = max_ms,
				.total_iterations = total_iters,
				.avg_iterations = avg_iters,
				.std_dev_ms = stddev_ms,
				.throughput_alg_per_s = throughput_per_s,
				.note = "Varies by algorithm"
			};
		}

		static summary compute_summary(const std::vector<visual_inspector::image_metrics>& image_metrics_list)
		{
			if (image_metrics_list.size() <= 1)
			{
				return {};
			}

			double total_ms = 0.0;
			double min_ms = std::numeric_limits<double>::max();
			double max_ms = 0.0;
			int64_t total_iterations = 0;
			int total_number = 0;

			for (size_t i = 1; i < image_metrics_list.size(); ++i)
			{
				const auto& image_metrics = image_metrics_list[i];

				if (image_metrics.execution_time_ms <= 0.0)
				{
					continue;
				}

				total_ms += image_metrics.execution_time_ms;
				min_ms = std::min(min_ms, image_metrics.execution_time_ms);
				max_ms = std::max(max_ms, image_metrics.execution_time_ms);
				total_iterations += image_metrics.iterations;
				++total_number;
			}

			if (total_number == 0)
			{
				return {};
			}

			const double avg_ms = total_ms / static_cast<double>(total_number);
			const int64_t avg_it = total_iterations / static_cast<int64_t>(total_number);

			double sum_sq = 0.0;

			if (total_number > 1)
			{
				for (size_t i = 1; i < image_metrics_list.size(); ++i)
				{
					const auto& image_metrics = image_metrics_list[i];

					if (image_metrics.execution_time_ms <= 0.0)
					{
						continue;
					}

					const auto d = image_metrics.execution_time_ms - avg_ms;

					sum_sq += d * d;
				}
			}

			const double stddev_ms = total_number > 1 ? std::sqrt(sum_sq / total_number) : 0.0;
			const double thr = avg_ms > 0.0 ? 1000.0 / avg_ms : 0.0;

			return summary{
				.avg_ms = avg_ms,
				.min_ms = (min_ms == std::numeric_limits<double>::max() ? 0.0 : min_ms),
				.max_ms = max_ms,
				.total_iters = total_iterations,
				.avg_iters = avg_it,
				.stddev_ms = stddev_ms,
				.throughput_per_s = thr
			};
		}

		static std::string extract_after_first_slash(const std::string& s)
		{
			const auto p = s.find('/');
			return p == std::string::npos ? s : s.substr(p + 1);
		}
	};
}
