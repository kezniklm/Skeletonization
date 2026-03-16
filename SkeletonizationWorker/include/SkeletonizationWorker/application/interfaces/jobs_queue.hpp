/**
*
* @file jobs_queue.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares the jobs queue contract used by the worker loop.
 *
 * This file defines queue interface for popping and acknowledging jobs.
 *
 * Main responsibilities:
 * - define queue pop contract
 * - define job acknowledgement contract
 * - define job requeue contract
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <chrono>
#include <expected>
#include <string>

namespace worker::application::interfaces
{
	/**
	 * @class i_jobs_queue
	 * @brief Defines the queue contract for polling and acknowledging jobs.
	 */
	class i_jobs_queue
	{
	public:
		/**
		 * @brief Destroys the jobs queue interface.
		 */
		virtual ~i_jobs_queue() = default;

		/**
		 * @brief Pops one job payload from the queue.
		 *
		 * @param timeout Maximum wait duration.
		 * @return Job payload on success or error message.
		 */
		virtual std::expected<std::string, std::string> pop_job(std::chrono::seconds timeout) = 0;
		/**
		 * @brief Acknowledges successful processing of a payload.
		 *
		 * @param job_payload Original queue payload.
		 * @return Empty result on success or error message.
		 */
		virtual std::expected<void, std::string> acknowledge_job(const std::string& job_payload) = 0;
		/**
		 * @brief Requeues a payload for later processing.
		 *
		 * @param job_payload Original queue payload.
		 * @return Empty result on success or error message.
		 */
		virtual std::expected<void, std::string> requeue_job(const std::string& job_payload) = 0;
	};
}
