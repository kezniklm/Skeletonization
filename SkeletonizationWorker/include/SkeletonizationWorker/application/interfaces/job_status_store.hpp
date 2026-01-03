#pragma once

#include <expected>
#include <string>

namespace worker::application::interfaces
{
	class i_job_status_store
	{
	public:
		virtual ~i_job_status_store() = default;
		virtual std::expected<void, std::string> update_job_status(const std::string& job_id, const std::string& status)
		= 0;
	};
}
