/**
*
* @file dependency_injection_installer.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares application-layer dependency injection installer.
 *
 * This file defines dependency injection installer for application services.
 *
 * Main responsibilities:
 * - register application service bindings
 * - wire use-case and service dependencies
 * - expose installer entry points for composition
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <boost/di.hpp>

#include "SkeletonizationWorker/application/interfaces/job_processor.hpp"
#include "SkeletonizationWorker/application/services/job_processor.hpp"

namespace worker::application::dependency_injection
{
	namespace di = boost::di;

	inline auto make_installer()
	{
		return di::make_injector(
			di::bind<interfaces::i_job_processor>.to<services::job_processor>()
		);
	}
}
