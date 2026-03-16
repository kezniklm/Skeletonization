/**
*
* @file i_arguments_provider.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares CLI arguments provider interface.
*
* This file defines the contract used to access parsed command-line
* arguments in CLI workflows.
*
* Main responsibilities:
* - expose parsed paths and benchmark options
* - provide benchmark iteration settings
* - provide parsed skeletonizer configurations
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "SkeletonizationCore/configuration/types.hpp"

namespace cli::interfaces
{
	/**
	 * @class i_arguments_provider
	 * @brief Interface for providing command-line arguments.
	 *
	 * Abstracts access to parsed command-line arguments, enabling
	 * dependency injection and testability.
	 */
	class i_arguments_provider
	{
	public:
		/**
		 * @brief Destroys the arguments provider interface.
		 */
		virtual ~i_arguments_provider() = default;

		/**
		 * @brief Returns configuration file path.
		 *
		 * @return Configuration path.
		 */
		[[nodiscard]] virtual const std::string& configuration_path() const = 0;
		/**
		 * @brief Returns benchmark output path.
		 *
		 * @return Benchmark output path.
		 */
		[[nodiscard]] virtual const std::string& benchmark_output_path() const = 0;
		/**
		 * @brief Returns number of benchmark iterations.
		 *
		 * @return Iteration count.
		 */
		[[nodiscard]] virtual unsigned int benchmark_iterations() const = 0;
		/**
		 * @brief Indicates whether preprocessing should run.
		 *
		 * @return True when preprocessing is enabled.
		 */
		[[nodiscard]] virtual bool run_image_preprocessing() const = 0;
		/**
		 * @brief Returns parsed skeletonizer configurations.
		 *
		 * @return Collection of skeletonizer configurations.
		 */
		[[nodiscard]] virtual const std::vector<configuration::skeletonizer_config>&
		skeletonizer_configurations() const = 0;

	protected:
		i_arguments_provider() = default;
		i_arguments_provider(const i_arguments_provider&) = default;
		i_arguments_provider& operator=(const i_arguments_provider&) = default;
		i_arguments_provider(i_arguments_provider&&) = default;
		i_arguments_provider& operator=(i_arguments_provider&&) = default;
	};
}
