/**
*
* @file configuration_installer.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares configuration dependency injection installer.
 *
 * This file defines dependency injection wiring for configuration services.
 *
 * Main responsibilities:
 * - register configuration loader bindings
 * - register parser and validation dependencies
 * - compose configuration service graph
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <memory>

#include <boost/di.hpp>

#include "SkeletonizationCLI/commandline/arguments.hpp"
#include "SkeletonizationCLI/configuration/loader.hpp"
#include "SkeletonizationCLI/interfaces/i_arguments_provider.hpp"
#include "SkeletonizationCLI/interfaces/i_configuration_loader.hpp"

namespace cli::dependency_injection
{
	namespace di = boost::di;

	/**
	 * @brief Create configuration dependency injector.
	 * @param args_provider Shared pointer to arguments provider for injection.
	 * @return Configured Boost.DI injector with configuration services.
	 */
	inline auto make_configuration_installer(
		std::shared_ptr<interfaces::i_arguments_provider> args_provider)
	{
		return di::make_injector(
			di::bind<interfaces::i_arguments_provider>.to(args_provider),
			di::bind<interfaces::i_configuration_loader>.to<configuration::configuration_loader>().in(di::singleton)
		);
	}
}
