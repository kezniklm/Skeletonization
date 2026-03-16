/**
*
* @file i_configuration_loader.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares configuration loader interface.
*
* This file defines the contract for loading benchmark configuration data
* from files and in-memory structures.
*
* Main responsibilities:
* - load configuration from file paths
* - load configuration from parsed objects
* - produce image benchmark metadata collections
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <string>
#include <vector>

#include "SkeletonizationCore/configuration/types.hpp"

namespace cli::interfaces
{
	/**
	 * @class i_configuration_loader
	 * @brief Interface for loading skeletonizer configuration.
	 *
	 * Abstracts the configuration loading mechanism, enabling
	 * different sources (file, memory, network) and testability.
	 */
	class i_configuration_loader
	{
	public:
		/**
		 * @brief Destroys the configuration loader interface.
		 */
		virtual ~i_configuration_loader() = default;

		/**
		 * @brief Load configuration from a file.
		 * @param filename Path to the JSON configuration file.
		 * @return Vector of parsed image benchmark metadata.
		 * @throws configuration_exception if loading fails.
		 */
		[[nodiscard]] virtual std::vector<configuration::image_benchmark_metadata>
		load(const std::string& filename) const = 0;

		/**
		 * @brief Load configuration from in-memory structures.
		 * @param configs Pre-parsed skeletonizer configurations.
		 * @return Vector of image benchmark metadata.
		 */
		[[nodiscard]] virtual std::vector<configuration::image_benchmark_metadata>
		load(const std::vector<configuration::skeletonizer_config>& configs) const = 0;

	protected:
		i_configuration_loader() = default;
		i_configuration_loader(const i_configuration_loader&) = default;
		i_configuration_loader& operator=(const i_configuration_loader&) = default;
		i_configuration_loader(i_configuration_loader&&) = default;
		i_configuration_loader& operator=(i_configuration_loader&&) = default;
	};
}
