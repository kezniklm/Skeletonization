#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "helpers.hpp"
#include "runner.hpp"


namespace skeletonization_benchmark
{
	class aggregator final
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

		static std::vector<package> build(
			const std::vector<std::pair<std::string, std::unique_ptr<runner>>>& runners,
			const metrics_map& metrics_map);
	};
}
