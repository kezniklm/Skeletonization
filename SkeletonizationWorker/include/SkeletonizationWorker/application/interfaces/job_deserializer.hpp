#pragma once

#include <expected>
#include <string>

#include "SkeletonizationWorker/domain/job.hpp"

namespace worker::application::interfaces
{
	class i_job_deserializer
	{
	public:
		virtual ~i_job_deserializer() = default;
		virtual std::expected<job::job, std::string> deserialize(const std::string& payload) = 0;
	};
}
