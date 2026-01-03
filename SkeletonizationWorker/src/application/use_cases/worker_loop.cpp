#include "SkeletonizationWorker/application/use_cases/worker_loop.hpp"

#include <glog/logging.h>

namespace worker::application::use_cases
{
	worker_loop::worker_loop(
		interfaces::i_jobs_queue& jobs_queue,
		interfaces::i_job_deserializer& job_deserializer,
		interfaces::i_job_processor& job_processor,
		interfaces::i_job_status_store& job_status_store,
		configuration::dependency_injection::cancellation_token_t& cancellation_token,
		const configuration::dependency_injection::poll_timeout_t poll_timeout)
		: jobs_queue_(jobs_queue),
		  job_deserializer_(job_deserializer),
		  job_processor_(job_processor),
		  job_status_store_(job_status_store),
		  cancellation_token_(cancellation_token),
		  poll_timeout_(poll_timeout.value)
	{
	}

	void worker_loop::run() const
	{
		LOG(INFO) << "Worker loop started";

		while (!cancellation_token_.is_cancellation_requested())
		{
			const auto job_payload_result = jobs_queue_.pop_job(poll_timeout_);

			if (!job_payload_result)
			{
				if (job_payload_result.error() == "timeout")
				{
					continue;
				}

				LOG(ERROR) << "Failed to pop job: " << job_payload_result.error();
				continue;
			}

			const auto& job_payload = job_payload_result.value();

			if (job_payload.empty())
			{
				continue;
			}

			process_payload(job_payload);
		}

		LOG(INFO) << "Worker loop stopped";
	}

	void worker_loop::process_payload(const std::string& job_payload) const
	{
		const auto job_data_result = job_deserializer_.deserialize(job_payload);

		if (!job_data_result)
		{
			LOG(ERROR) << "Failed to parse job JSON: " << job_data_result.error();

			const auto ack_result = jobs_queue_.acknowledge_job(job_payload);
			if (!ack_result)
			{
				LOG(ERROR) << "Failed to acknowledge bad job: " << ack_result.error();
			}

			return;
		}

		const job::job& job = job_data_result.value();

		LOG(INFO) << "Processing job: " << job.id;

		const auto update_processing_status = job_status_store_.update_job_status(job.id, "processing");

		if (!update_processing_status)
		{
			LOG(ERROR) << "Failed to update job status to 'processing': " << update_processing_status.error();

			return;
		}

		const auto process_result = job_processor_.process_job(job);

		if (!process_result)
		{
			const auto update_failed_status = job_status_store_.update_job_status(job.id, "failed");

			if (!update_failed_status)
			{
				LOG(ERROR) << "Failed to update job status to 'failed': " << update_failed_status.error();
			}

			const auto job_acknowledge_result = jobs_queue_.acknowledge_job(job_payload);

			if (!job_acknowledge_result)
			{
				LOG(ERROR) << "Failed to acknowledge failed job: " << job_acknowledge_result.error();
			}

			return;
		}

		const auto update_status_completed = job_status_store_.update_job_status(job.id, "completed");

		const auto job_acknowledge_result = jobs_queue_.acknowledge_job(job_payload);

		if (!job_acknowledge_result)
		{
			LOG(ERROR) << "Failed to acknowledge failed job: " << job_acknowledge_result.error();
		}
	}
}
