/**
*
* @file dependency_injection.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares worker dependency injection composition helpers.
 *
 * This file defines top-level dependency injection composition for worker.
 *
 * Main responsibilities:
 * - compose worker service graph
 * - install domain, application, and infrastructure modules
 * - expose configured injector creation helpers
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <boost/di.hpp>

#include "SkeletonizationWorker/application/dependency_injection_installer.hpp"
#include "SkeletonizationWorker/configuration.hpp"
#include "SkeletonizationWorker/domain/dependency_injection_installer.hpp"
#include "SkeletonizationWorker/infrastructure/dependency_injection_installer.hpp"

namespace worker::dependency_injection
{
	namespace di = boost::di;

	inline auto configure_dependency_injection(const configuration::configuration& configuration)
	{
		return di::make_injector(
			domain::dependency_injection::make_installer(),
			application::dependency_injection::make_installer(),
			infrastructure::dependency_injection::make_installer(configuration)
		);
	}
}
