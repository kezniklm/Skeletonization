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
