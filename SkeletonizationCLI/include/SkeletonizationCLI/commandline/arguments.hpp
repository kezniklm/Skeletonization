#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "SkeletonizationCLI/interfaces/i_arguments_provider.hpp"
#include "SkeletonizationCore/configuration/types.hpp"

namespace commandline
{
	inline constexpr std::string_view default_configuration_path =
		"../skeletonizer_config.json";

	/**
	 * @brief Raw argument data structure.
	 *
	 * Contains the parsed command-line arguments. Used internally
	 * by the arguments_provider.
	 */
	struct arguments
	{
		std::string configuration_path = std::string(default_configuration_path);

		std::string benchmark_out;

		unsigned int number_of_benchmark_iterations = 1000;

		std::vector<configuration::skeletonizer_config> skeletonizer_configurations;

		bool run_image_preprocessing{};
	};

	/**
	 * @brief Concrete implementation of i_arguments_provider.
	 *
	 * Wraps the argument struct and provides interface-based access
	 * for dependency injection.
	 */
	class arguments_provider final : public cli::interfaces::i_arguments_provider
	{
	public:
		arguments_provider() = default;

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

		[[nodiscard]] const arguments& args() const noexcept
		{
			return args_;
		}

	private:
		arguments args_;
	};
}
