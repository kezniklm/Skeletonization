/**
*
* @file arguments.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares CLI argument access helpers.
*
* This file defines argument data models and provider implementation for
* exposing parsed command-line values.
*
* Main responsibilities:
* - define parsed CLI argument data structure
* - provide interface-based argument access
* - expose benchmark and configuration settings
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "SkeletonizationCLI/interfaces/i_arguments_provider.hpp"
#include "SkeletonizationCore/configuration/types.hpp"

namespace commandline
{
	/// Default path for configuration input.
	inline constexpr std::string_view default_configuration_path =
		"../skeletonizer_config.json";

	/**
	 * @class arguments
	 * @brief Raw argument data structure.
	 *
	 * Contains the parsed command-line arguments. Used internally
	 * by the arguments_provider.
	 */
	struct arguments
	{
		/// Path to configuration file.
		std::string configuration_path = std::string(default_configuration_path);

		/// Path prefix for benchmark output.
		std::string benchmark_out;

		/// Number of benchmark iterations.
		unsigned int number_of_benchmark_iterations = 1000;

		/// Parsed skeletonizer configurations.
		std::vector<configuration::skeletonizer_config> skeletonizer_configurations;

		/// Enables image preprocessing before execution.
		bool run_image_preprocessing;
	};

	/**
	 * @class arguments_provider
	 * @brief Concrete implementation of i_arguments_provider.
	 *
	 * Wraps the argument struct and provides interface-based access
	 * for dependency injection.
	 */
	class arguments_provider final : public cli::interfaces::i_arguments_provider
	{
	public:
		/**
		 * @brief Constructs arguments provider with default values.
		 */
		arguments_provider() = default;

		/**
		 * @brief Constructs arguments provider from parsed data.
		 *
		 * @param args Parsed argument structure.
		 */
		explicit arguments_provider(arguments args)
			: args_(std::move(args))
		{
		}

		[[nodiscard]] const std::string& configuration_path() const override
		{
			return args_.configuration_path;
		}

		[[nodiscard]] const std::string& benchmark_output_path() const override
		{
			return args_.benchmark_out;
		}

		[[nodiscard]] unsigned int benchmark_iterations() const override
		{
			return args_.number_of_benchmark_iterations;
		}

		[[nodiscard]] bool run_image_preprocessing() const override
		{
			return args_.run_image_preprocessing;
		}

		[[nodiscard]] const std::vector<configuration::skeletonizer_config>&
		skeletonizer_configurations() const override
		{
			return args_.skeletonizer_configurations;
		}

		/**
		 * @brief Returns full parsed argument structure.
		 *
		 * @return Parsed argument structure.
		 */
		[[nodiscard]] const arguments& args() const noexcept
		{
			return args_;
		}

	private:
		/// Parsed argument storage.
		arguments args_;
	};
}
