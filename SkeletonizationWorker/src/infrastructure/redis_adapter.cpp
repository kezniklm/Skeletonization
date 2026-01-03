#include <glog/logging.h>

#include "SkeletonizationWorker/configuration.hpp"
#include "SkeletonizationWorker/infrastructure/redis.hpp"
#include "SkeletonizationWorker/infrastructure/redis_adapter.hpp"

namespace worker::infrastructure
{
	redis_adapter::redis_adapter(redis::client& client,
	                             configuration::dependency_injection::jobs_queue_t jobs_queue,
	                             configuration::dependency_injection::processing_queue_t processing_queue,
	                             configuration::dependency_injection::results_queue_t results_queue)
		: client_(client),
		  jobs_queue_(std::move(jobs_queue.value)),
		  processing_queue_(std::move(processing_queue.value)),
		  results_queue_(std::move(results_queue.value))
	{
	}

	std::expected<std::string, std::string> redis_adapter::pop_job(const std::chrono::seconds timeout)
	{
		const auto ensure_result = client_.ensure_connected();

		if (!ensure_result)
		{
			return std::unexpected(ensure_result.error());
		}

		return client_.pop_safe(jobs_queue_, processing_queue_, timeout);
	}

	std::expected<void, std::string> redis_adapter::acknowledge_job(const std::string& job_payload)
	{
		return client_.acknowledge_job(processing_queue_, job_payload);
	}

	std::expected<void, std::string> redis_adapter::requeue_job(const std::string& job_payload)
	{
		return client_.requeue(jobs_queue_, job_payload);
	}

	std::expected<void, std::string> redis_adapter::publish_result(const std::string& result_payload)
	{
		const auto ensure_result = client_.ensure_connected();

		if (!ensure_result)
		{
			return std::unexpected(ensure_result.error());
		}

		const auto result = client_.requeue(results_queue_, result_payload);

		if (!result)
		{
			return std::unexpected("Failed to publish result: " + result.error());
		}

		LOG(INFO) << "Published result to queue: " << results_queue_;

		return {};
	}

	std::expected<void, std::string> redis_adapter::update_job_status(const std::string& job_id,
	                                                                  const std::string& status)
	{
		const std::string key = "job:status:" + job_id;

		return client_.set(key, status);
	}
}
