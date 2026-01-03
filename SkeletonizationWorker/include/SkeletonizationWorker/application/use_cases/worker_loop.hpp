#pragma once

#include <chrono>
#include <string>

#include "SkeletonizationWorker/application/interfaces/job_deserializer.hpp"
#include "SkeletonizationWorker/application/interfaces/job_processor.hpp"
#include "SkeletonizationWorker/application/interfaces/jobs_queue.hpp"
#include "SkeletonizationWorker/application/interfaces/job_status_store.hpp"
#include "SkeletonizationWorker/configuration.hpp"

namespace worker::application::use_cases
{
	class worker_loop
	{
	public:
		worker_loop(
			interfaces::i_jobs_queue& jobs_queue,
			interfaces::i_job_deserializer& job_deserializer,
			interfaces::i_job_processor& job_processor,
			interfaces::i_job_status_store& job_status_store,
			configuration::dependency_injection::cancellation_token_t& cancellation_token,
			configuration::dependency_injection::poll_timeout_t poll_timeout);

		void run() const;

	private:
		interfaces::i_jobs_queue& jobs_queue_;
		interfaces::i_job_deserializer& job_deserializer_;
		interfaces::i_job_processor& job_processor_;
		interfaces::i_job_status_store& job_status_store_;
		configuration::dependency_injection::cancellation_token_t& cancellation_token_;
		std::chrono::seconds poll_timeout_;

		void process_payload(const std::string& job_payload) const;
	};
}
