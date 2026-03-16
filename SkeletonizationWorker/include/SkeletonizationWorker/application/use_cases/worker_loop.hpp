/**
*
* @file worker_loop.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares the worker loop use-case service.
*
* This file defines the worker polling loop use case that fetches jobs,
* deserializes payloads, and executes processing.
*
* Main responsibilities:
* - poll queue for new job payloads
* - deserialize and process job payloads
* - update job status during processing
*
* @version 1.0
* @date 2026-03-16
*/

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
	/**
	 * @class worker_loop
	 * @brief Implements continuous worker queue processing loop.
	 */
	class worker_loop
	{
	public:
		/**
		 * @brief Constructs worker loop with processing dependencies.
		 * @param jobs_queue Queue interface used for polling jobs.
		 * @param job_deserializer Job payload deserializer.
		 * @param job_processor Job processing service.
		 * @param job_status_store Job status persistence service.
		 * @param cancellation_token Shared cancellation state.
		 * @param poll_timeout Queue polling timeout configuration.
		 */
		worker_loop(
			interfaces::i_jobs_queue& jobs_queue,
			interfaces::i_job_deserializer& job_deserializer,
			interfaces::i_job_processor& job_processor,
			interfaces::i_job_status_store& job_status_store,
			configuration::dependency_injection::cancellation_token_t& cancellation_token,
			configuration::dependency_injection::poll_timeout_t poll_timeout);

		/**
		 * @brief Runs worker loop until cancellation is requested.
		 */
		void run() const;

	private:
		/// Jobs queue dependency.
		interfaces::i_jobs_queue& jobs_queue_;
		/// Job deserializer dependency.
		interfaces::i_job_deserializer& job_deserializer_;
		/// Job processor dependency.
		interfaces::i_job_processor& job_processor_;
		/// Job status store dependency.
		interfaces::i_job_status_store& job_status_store_;
		/// Cancellation token reference.
		configuration::dependency_injection::cancellation_token_t& cancellation_token_;
		/// Queue polling timeout.
		std::chrono::seconds poll_timeout_;

		/**
		 * @brief Processes one serialized job payload.
		 *
		 * @param job_payload Serialized job payload.
		 */
		void process_payload(const std::string& job_payload) const;
	};
}
