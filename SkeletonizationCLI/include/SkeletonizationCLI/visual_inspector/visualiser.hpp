#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

#include "image_container.hpp"

namespace visual_inspector
{
	void open_in_browser(const std::string& url);

	struct image_metrics
	{
		int64_t iterations = 0;
		double real_time = 0.0; // nanoseconds
		double cpu_time = 0.0; // nanoseconds
		std::string time_unit = "ns"; // "ns", "us", "ms", "s"
		double bytes_per_second = 0.0;
		double items_per_second = 0.0;

		double execution_time_ms = 0.0;
		double memory_usage_mb = 0.0;
		int pixel_count = 0;
		int non_zero_pixels = 0;
		double compression_ratio = 0.0;
	};

	class visualiser
	{
	public:
		void add_benchmark_image_container(const image_container& image_container);

		static void show(const std::filesystem::path& web_root_or_index, uint16_t port = 8787);

	private:
		std::vector<image_container> benchmark_image_containers_;
		std::vector<std::vector<image_metrics>> container_metrics_;
	};
}
