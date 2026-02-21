#pragma once

#include <string>
#include <vector>

#include "SkeletonizationCLI/interfaces/i_configuration_loader.hpp"
#include "SkeletonizationCore/configuration/types.hpp"

namespace configuration
{
	/**
	 * @brief Concrete implementation of i_configuration_loader.
	 *
	 * Loads skeletonizer configuration from JSON files or in-memory structures.
	 */
	class configuration_loader final : public cli::interfaces::i_configuration_loader
	{
	public:
		[[nodiscard]] std::vector<image_benchmark_metadata>
		load(const std::string& filename) const override;

		[[nodiscard]] std::vector<image_benchmark_metadata>
		load(const std::vector<skeletonizer_config>& configs) const override;
	};
}
