#pragma once

#include <filesystem>
#include <vector>

#include "SkeletonizationCore/configuration/types.hpp"

namespace skeletonization_benchmark
{
	class aggregator;
}

namespace cli::interfaces
{
	/**
	 * @brief Interface for exporting benchmark results.
	 *
	 * Abstracts the export mechanism to allow different output
	 * formats and destinations.
	 */
	class i_exporter
	{
	public:
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
