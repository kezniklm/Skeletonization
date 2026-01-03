#pragma once

#include <expected>
#include <string>

#include "SkeletonizationWorker/domain/job.hpp"

namespace worker::application::interfaces
{
	class i_job_processor
	{
	public:
		virtual ~i_job_processor() = default;
		virtual std::expected<void, std::string> process_job(const job::job& job_data) = 0;
	};
}
