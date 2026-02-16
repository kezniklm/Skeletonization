#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "aggregator.hpp"
#include "SkeletonizationCore/configuration/types.hpp"

namespace skeletonization_benchmark
{
	struct exporter_options
	{
		// Output formatting
		bool pretty = true;

		// Content toggles
		bool include_algorithm_info = true;
		bool include_metrics = true;

		// Image encoding
		std::string image_ext = ".png"; // ".png", ".jpg", ...
		int png_compression = 3; // 0..9 (if PNG)
		int jpeg_quality = 90; // 1..100 (if JPEG)

		// File writing
		bool atomic_write = true; // write to temp + rename
	};

	class exporter final
	{
	public:
		[[nodiscard]] static bool write_output_json(
			const std::vector<aggregator::package>& packages,
			const std::filesystem::path& out_json_path,
			const exporter_options& opts = {});

		[[nodiscard]] static bool write_configuration_json(
			const std::vector<configuration::image_benchmark_metadata>& configs,
			const std::filesystem::path& out_json_path,
			const exporter_options& opts = {});

		[[nodiscard]] static std::filesystem::path create_timestamped_output_path(
			const std::string& filename);
	};
}
