/**
*
* @file job_processor.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares the job processing service contract.
 *
 * This file defines interface for processing deserialized job models.
 *
 * Main responsibilities:
 * - define job processing contract
 * - abstract processing implementation details
 * - surface processing failures as expected values
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <expected>
#include <string>

#include "SkeletonizationWorker/domain/job.hpp"

namespace worker::application::interfaces
{
	/**
	 * @class i_job_processor
	 * @brief Defines the contract for processing deserialized jobs.
	 */
	class i_job_processor
	{
	public:
		/**
		 * @brief Destroys the job processor interface.
		 */
		virtual ~i_job_processor() = default;
		/**
		 * @brief Processes a domain job payload.
		 *
		 * @param job_data Deserialized job model.
		 * @return Empty result on success or error message on failure.
		 */
		virtual std::expected<void, std::string> process_job(const job::job& job_data) = 0;
	};
}
