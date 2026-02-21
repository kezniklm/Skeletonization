#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "SkeletonizationCore/configuration/types.hpp"

namespace cli::interfaces
{
	/**
	 * @brief Interface for providing command-line arguments.
	 *
	 * Abstracts access to parsed command-line arguments, enabling
	 * dependency injection and testability.
	 */
	class i_arguments_provider
	{
	public:
		virtual ~i_arguments_provider() = default;

		[[nodiscard]] virtual const std::string& configuration_path() const = 0;
		[[nodiscard]] virtual const std::string& benchmark_output_path() const = 0;
		[[nodiscard]] virtual unsigned int benchmark_iterations() const = 0;
		[[nodiscard]] virtual bool run_image_preprocessing() const = 0;
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
