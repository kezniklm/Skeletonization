/**
*
* @file i_exporter.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares exporter interface for benchmark outputs.
*
* This file defines the contract for exporting benchmark outputs and
* configuration metadata.
*
* Main responsibilities:
* - export aggregated benchmark results
* - export benchmark configuration metadata
* - abstract output mechanism from callers
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <filesystem>
#include <vector>

#include "SkeletonizationCore/configuration/types.hpp"

namespace skeletonization_benchmark
{
	/**
	 * @class aggregator
	 * @brief Forward declaration of benchmark aggregation type.
	 */
	class aggregator;
}

namespace cli::interfaces
{
	/**
	 * @class i_exporter
	 * @brief Interface for exporting benchmark results.
	 *
	 * Abstracts the export mechanism to allow different output
	 * formats and destinations.
	 */
	class i_exporter
	{
	public:
		/**
		 * @brief Destroys the exporter interface.
		 */
		virtual ~i_exporter() = default;

		/**
		 * @brief Export benchmark packages to a JSON file.
		 * @param packages The aggregated benchmark data.
		 * @param output_path Destination file path.
		 * @return true if export succeeded, false otherwise.
		 */
		[[nodiscard]] virtual bool export_results(
			const std::vector<skeletonization_benchmark::aggregator::package>& packages,
			const std::filesystem::path& output_path) const = 0;

		/**
		 * @brief Export configuration metadata to a JSON file.
		 * @param configs The configuration metadata to export.
		 * @param output_path Destination file path.
		 * @return true if export succeeded, false otherwise.
		 */
		[[nodiscard]] virtual bool export_configuration(
			const std::vector<configuration::image_benchmark_metadata>& configs,
			const std::filesystem::path& output_path) const = 0;

	protected:
		i_exporter() = default;
		i_exporter(const i_exporter&) = default;
		i_exporter& operator=(const i_exporter&) = default;
		i_exporter(i_exporter&&) = default;
		i_exporter& operator=(i_exporter&&) = default;
	};
}
