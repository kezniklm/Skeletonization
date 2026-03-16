/**
*
* @file exporter.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares benchmark export services.
*
* This file defines export options and exporter services for writing
* benchmark outputs and configuration metadata.
*
* Main responsibilities:
* - define output export option set
* - export benchmark packages to JSON
* - export configuration metadata to JSON
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "aggregator.hpp"
#include "SkeletonizationCLI/interfaces/i_exporter.hpp"
#include "SkeletonizationCore/configuration/types.hpp"

namespace skeletonization_benchmark
{
	/**
	 * @class exporter_options
	 * @brief Configures exporter behavior and output formatting.
	 */
	struct exporter_options
	{
		/// Enables pretty JSON output formatting.
		bool pretty = true;

		/// Includes algorithm information in exported output.
		bool include_algorithm_info = true;
		/// Includes metrics in exported output.
		bool include_metrics = true;

		/// Output image extension.
		std::string image_ext = ".png";
		/// PNG compression level.
		int png_compression = 3;
		/// JPEG quality level.
		int jpeg_quality = 90;

		/// Enables atomic file writes through temp + rename.
		bool atomic_write = true;
	};

	/**
	 * @class exporter
	 * @brief Default implementation of i_exporter interface.
	 *
	 * Exports benchmark results and configuration to JSON files.
	 * Can be used both via dependency injection or via static methods
	 * for backward compatibility.
	 */
	class exporter final : public cli::interfaces::i_exporter
	{
	public:
		/**
		 * @brief Exports benchmark result packages.
		 *
		 * @param packages Aggregated benchmark result packages.
		 * @param output_path Output file path.
		 * @return True when export succeeds.
		 */
		[[nodiscard]] bool export_results(
			const std::vector<aggregator::package>& packages,
			const std::filesystem::path& output_path) const override
		{
			return write_output_json(packages, output_path);
		}

		/**
		 * @brief Exports benchmark configuration metadata.
		 *
		 * @param configs Benchmark configuration metadata.
		 * @param output_path Output file path.
		 * @return True when export succeeds.
		 */
		[[nodiscard]] bool export_configuration(
			const std::vector<configuration::image_benchmark_metadata>& configs,
			const std::filesystem::path& output_path) const override
		{
			return write_configuration_json(configs, output_path);
		}

		/**
		 * @brief Writes benchmark output JSON file.
		 *
		 * @param packages Aggregated benchmark packages.
		 * @param out_json_path Output JSON path.
		 * @param opts Export options.
		 * @return True when write succeeds.
		 */
		[[nodiscard]] static bool write_output_json(
			const std::vector<aggregator::package>& packages,
			const std::filesystem::path& out_json_path,
			const exporter_options& opts = {});

		/**
		 * @brief Writes configuration JSON file.
		 *
		 * @param configs Configuration metadata collection.
		 * @param out_json_path Output JSON path.
		 * @param opts Export options.
		 * @return True when write succeeds.
		 */
		[[nodiscard]] static bool write_configuration_json(
			const std::vector<configuration::image_benchmark_metadata>& configs,
			const std::filesystem::path& out_json_path,
			const exporter_options& opts = {});

		/**
		 * @brief Creates timestamped output path for exported files.
		 *
		 * @param filename Base filename.
		 * @return Timestamped output path.
		 */
		[[nodiscard]] static std::filesystem::path create_timestamped_output_path(
			const std::string& filename);
	};
}
