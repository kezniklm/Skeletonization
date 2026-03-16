/**
*
* @file loader.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares configuration loading services.
*
* This file defines configuration loader implementation used to build
* benchmark metadata from files and in-memory values.
*
* Main responsibilities:
* - load configuration metadata from files
* - load configuration metadata from parsed values
* - produce benchmark-ready metadata structures
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <string>
#include <vector>

#include "SkeletonizationCLI/interfaces/i_configuration_loader.hpp"
#include "SkeletonizationCore/configuration/types.hpp"

namespace configuration
{
	/**
	 * @class configuration_loader
	 * @brief Concrete implementation of i_configuration_loader.
	 *
	 * Loads skeletonizer configuration from JSON files or in-memory structures.
	 */
	class configuration_loader final : public cli::interfaces::i_configuration_loader
	{
	public:
		/**
		 * @brief Loads configuration metadata from file.
		 *
		 * @param filename Configuration filename.
		 * @return Loaded image benchmark metadata.
		 */
		[[nodiscard]] std::vector<image_benchmark_metadata>
		load(const std::string& filename) const override;

		/**
		 * @brief Loads configuration metadata from parsed values.
		 *
		 * @param configs Parsed skeletonizer configurations.
		 * @return Loaded image benchmark metadata.
		 */
		[[nodiscard]] std::vector<image_benchmark_metadata>
		load(const std::vector<skeletonizer_config>& configs) const override;
	};
}
