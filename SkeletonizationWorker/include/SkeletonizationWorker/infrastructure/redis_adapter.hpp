/**
*
* @file redis_adapter.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares hiredis adapter helpers used by worker infrastructure.
*
* This file defines adapter bridging worker queue/status/result interfaces
* to Redis client operations.
*
* Main responsibilities:
* - adapt queue operations to Redis commands
* - adapt status updates to Redis key-value operations
* - adapt result transport to Redis publish queue
*
* @version 1.0
* @date 2026-03-16
*/

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
	/**
	 * @class redis_adapter
	 * @brief Adapts worker interfaces to Redis client operations.
	 */
	class redis_adapter final : public application::interfaces::i_jobs_queue,
	                            public application::interfaces::i_result_transport,
	                            public application::interfaces::i_job_status_store
	{
	public:
		/**
		 * @brief Constructs Redis adapter with queue names.
		 * @param client Redis client dependency.
		 * @param jobs_queue Queue name used for pending jobs.
		 * @param processing_queue Queue name used for in-progress jobs.
		 * @param results_queue Queue name used for published results.
		 */
		explicit redis_adapter(
			redis::client& client,
			configuration::dependency_injection::jobs_queue_t jobs_queue,
			configuration::dependency_injection::processing_queue_t processing_queue,
			configuration::dependency_injection::results_queue_t results_queue);

		/**
		 * @brief Pops one job payload from jobs queue.
		 * @param timeout Maximum wait duration.
		 * @return Job payload on success or error message on failure.
		 */
		std::expected<std::string, std::string> pop_job(std::chrono::seconds timeout) override;
		/**
		 * @brief Acknowledges successfully processed job.
		 * @param job_payload Original queue payload.
		 * @return Empty result on success or error message on failure.
		 */
		std::expected<void, std::string> acknowledge_job(const std::string& job_payload) override;
		/**
		 * @brief Requeues failed job payload back to jobs queue.
		 * @param job_payload Original queue payload.
		 * @return Empty result on success or error message on failure.
		 */
		std::expected<void, std::string> requeue_job(const std::string& job_payload) override;
		/**
		 * @brief Publishes serialized result payload.
		 * @param result_payload Serialized result payload.
		 * @return Empty result on success or error message on failure.
		 */
		std::expected<void, std::string> publish_result(const std::string& result_payload) override;
		/**
		 * @brief Updates persisted status for a job id.
		 * @param job_id Job identifier.
		 * @param status Job status value.
		 * @return Empty result on success or error message on failure.
		 */
		std::expected<void, std::string>
		update_job_status(const std::string& job_id, const std::string& status) override;

	private:
		/// Redis client dependency.
		redis::client& client_;
		/// Jobs queue name.
		std::string jobs_queue_;
		/// Processing queue name.
		std::string processing_queue_;
		/// Results queue name.
		std::string results_queue_;
	};
}
