#pragma once

#include "SkeletonizationWorker/application/interfaces/job_deserializer.hpp"

namespace worker::infrastructure
{
	class rapidjson_job_deserializer final : public application::interfaces::i_job_deserializer
	{
	public:
		std::expected<job::job, std::string> deserialize(const std::string& payload) override;
	};
}
