/**
*
* @file aggregator.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares benchmark aggregation utilities.
*
* This file defines aggregation structures and routines that combine
* benchmark runner outputs into presentation-ready packages.
*
* Main responsibilities:
* - define package model for aggregated benchmark data
* - combine runner outputs with parsed metrics
* - prepare summary statistics for result presentation
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "helpers.hpp"
#include "runner.hpp"


namespace skeletonization_benchmark
{
	/**
	 * @class aggregator
	 * @brief Aggregates benchmark outputs into result packages.
	 */
	class aggregator final
	{
	public:
		/**
		 * @class package
		 * @brief Stores one aggregated benchmark result package.
		 */
		struct package
		{
			/// Input/output images for visualization.
			visual_inspector::image_container container;
			/// Parsed per-algorithm benchmark metrics.
			std::vector<visual_inspector::image_metrics> metrics_list;
			/// Human-readable package description.
			std::string description;

			/// Average execution time in milliseconds.
			double avg_time_ms = 0.0;
			/// Minimum execution time in milliseconds.
			double min_time_ms = 0.0;
			/// Maximum execution time in milliseconds.
			double max_time_ms = 0.0;
			/// Sum of iteration counts.
			int64_t total_iterations = 0;
			/// Average iteration count.
			int64_t avg_iterations = 0;
			/// Standard deviation of execution time.
			double std_dev_ms = 0.0;
			/// Throughput in algorithms per second.
			double throughput_alg_per_s = 0.0;
			/// Optional note for reporting.
			std::string note;
		};

		/**
		 * @brief Builds aggregated packages from runners and metrics.
		 *
		 * @param runners Registered benchmark runners.
		 * @param metrics_map Parsed benchmark metrics map.
		 * @return Aggregated result packages.
		 */
		static std::vector<package> build(
			const std::vector<std::pair<std::string, std::unique_ptr<runner>>>& runners,
			const metrics_map& metrics_map);
	};
}
