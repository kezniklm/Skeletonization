/**
*
* @file job_deserializer.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares the job deserialization contract.
 *
 * This file defines interface for deserializing serialized job payloads.
 *
 * Main responsibilities:
 * - define payload deserialization contract
 * - map payload text to domain job model
 * - surface parse errors through expected values
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
	 * @class i_job_deserializer
	 * @brief Defines the contract for converting payloads to domain jobs.
	 */
	class i_job_deserializer
	{
	public:
		/**
		 * @brief Destroys the job deserializer interface.
		 */
		virtual ~i_job_deserializer() = default;
		/**
		 * @brief Deserializes a payload into a domain job.
		 *
		 * @param payload Serialized job payload.
		 * @return Parsed domain job on success or error message.
		 */
		virtual std::expected<job::job, std::string> deserialize(const std::string& payload) = 0;
	};
}
