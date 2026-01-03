#pragma once

#include <chrono>
#include <expected>
#include <string>

namespace worker::application::interfaces
{
	class i_jobs_queue
	{
	public:
		virtual ~i_jobs_queue() = default;

		virtual std::expected<std::string, std::string> pop_job(std::chrono::seconds timeout) = 0;
		virtual std::expected<void, std::string> acknowledge_job(const std::string& job_payload) = 0;
		virtual std::expected<void, std::string> requeue_job(const std::string& job_payload) = 0;
	};
}
