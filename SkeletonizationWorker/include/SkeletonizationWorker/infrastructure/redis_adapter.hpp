#pragma once

#include <chrono>
#include <expected>
#include <string>

#include "SkeletonizationWorker/configuration.hpp"
#include "SkeletonizationWorker/infrastructure/redis.hpp"

#include "SkeletonizationWorker/application/interfaces/jobs_queue.hpp"
#include "SkeletonizationWorker/application/interfaces/job_status_store.hpp"
#include "SkeletonizationWorker/application/interfaces/result_transport.hpp"

namespace worker::infrastructure
{
	class redis_adapter final : public application::interfaces::i_jobs_queue,
	                            public application::interfaces::i_result_transport,
	                            public application::interfaces::i_job_status_store
	{
	public:
		explicit redis_adapter(
			redis::client& client,
			configuration::dependency_injection::jobs_queue_t jobs_queue,
			configuration::dependency_injection::processing_queue_t processing_queue,
			configuration::dependency_injection::results_queue_t results_queue);

		std::expected<std::string, std::string> pop_job(std::chrono::seconds timeout) override;
		std::expected<void, std::string> acknowledge_job(const std::string& job_payload) override;
		std::expected<void, std::string> requeue_job(const std::string& job_payload) override;
		std::expected<void, std::string> publish_result(const std::string& result_payload) override;
		std::expected<void, std::string>
		update_job_status(const std::string& job_id, const std::string& status) override;

	private:
		redis::client& client_;
		std::string jobs_queue_;
		std::string processing_queue_;
		std::string results_queue_;
	};
}
