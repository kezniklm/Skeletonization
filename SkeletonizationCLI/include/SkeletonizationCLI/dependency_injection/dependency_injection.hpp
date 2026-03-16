/**
*
* @file dependency_injection.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares CLI dependency injection composition.
 *
 * This file defines top-level dependency injection composition for CLI.
 *
 * Main responsibilities:
 * - compose CLI dependency graph
 * - install module-level service installers
 * - expose configured injector entry points
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <memory>

#include <boost/di.hpp>

#include "SkeletonizationCLI/commandline/arguments.hpp"
#include "SkeletonizationCLI/dependency_injection/configuration_installer.hpp"
#include "SkeletonizationCLI/dependency_injection/benchmark_installer.hpp"
#include "SkeletonizationCLI/interfaces/i_arguments_provider.hpp"

namespace cli::dependency_injection
{
	namespace di = boost::di;

	/**
	 * @brief Configure all dependency injection with parsed arguments.
	 * @param parsed_args Parsed command-line arguments.
	 * @return Configured Boost.DI injector instance.
	 */
	inline auto configure(const commandline::arguments& parsed_args)
	{
		auto args_provider = std::make_shared<commandline::arguments_provider>(parsed_args);
		return di::make_injector(
			make_configuration_installer(args_provider),
			make_benchmark_installer()
		);
	}
}
