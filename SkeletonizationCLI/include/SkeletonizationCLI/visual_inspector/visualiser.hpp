/**
*
* @file visualiser.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares visual inspection rendering services.
*
* This file defines visualizer services, benchmark metrics structures,
* and browser-opening helpers for result visualization.
*
* Main responsibilities:
* - define benchmark metrics model
* - store visualization containers and metrics
* - render visualization through visualizer interface
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

#include "image_container.hpp"
#include "SkeletonizationCLI/interfaces/i_visualizer.hpp"

namespace visual_inspector
{
	/**
	 * @brief Opens URL in browser for visualization access.
	 *
	 * @param url URL to open.
	 */
	void open_in_browser(const std::string& url);

	/**
	 * @class image_metrics
	 * @brief Stores benchmark metric values for one image result.
	 */
	struct image_metrics
	{
		/// Iteration count.
		int64_t iterations = 0;
		/// Real elapsed time in nanoseconds.
		double real_time = 0.0;
		/// CPU time in nanoseconds.
		double cpu_time = 0.0;
		/// Time unit token.
		std::string time_unit = "ns";
		/// Processed bytes per second.
		double bytes_per_second = 0.0;
		/// Processed items per second.
		double items_per_second = 0.0;

		/// Execution time in milliseconds.
		double execution_time_ms = 0.0;
		/// Memory usage in megabytes.
		double memory_usage_mb = 0.0;
		/// Total pixel count.
		int pixel_count = 0;
		/// Non-zero pixel count.
		int non_zero_pixels = 0;
		/// Compression ratio metric.
		double compression_ratio = 0.0;
	};

	/**
	 * @class visualiser
	 * @brief Visualizer for benchmark results using web interface.
	 *
	 * Implements i_visualizer interface for dependency injection.
	 */
	class visualiser final : public cli::interfaces::i_visualizer
	{
	public:
		/**
		 * @brief Adds benchmark image container.
		 *
		 * @param image_container Benchmark image container.
		 */
		void add_benchmark_image_container(const image_container& image_container);

		/**
		 * @brief Display the visualization web interface.
		 * @param web_root Path to visualizer web assets.
		 * @param port Local HTTP port for visualization server.
		 */
		void show(const std::filesystem::path& web_root, std::uint16_t port = 8787) override;

	private:
		/// Benchmark image container list.
		std::vector<image_container> benchmark_image_containers_;
		/// Metrics per benchmark container.
		std::vector<std::vector<image_metrics>> container_metrics_;
	};
}
