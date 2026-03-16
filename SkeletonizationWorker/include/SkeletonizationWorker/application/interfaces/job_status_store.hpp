/**
*
* @file job_status_store.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares the job status persistence contract.
 *
 * This file defines interface for persisting job status transitions.
 *
 * Main responsibilities:
 * - define job status update contract
 * - abstract status persistence backend
 * - surface persistence failures as expected values
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <expected>
#include <string>

namespace worker::application::interfaces
{
	/**
	 * @class i_job_status_store
	 * @brief Defines persistence operations for job status updates.
	 */
	class i_job_status_store
	{
	public:
		/**
		 * @brief Destroys the status store interface.
		 */
		virtual ~i_job_status_store() = default;
		/**
		 * @brief Updates status value for a job identifier.
		 *
		 * @param job_id Job identifier.
		 * @param status New status value.
		 * @return Empty result on success or error message.
		 */
		virtual std::expected<void, std::string> update_job_status(const std::string& job_id, const std::string& status)
		= 0;
	};
}
