/**
*
* @file rapidjson_job_deserializer.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares RapidJSON-based job deserialization implementation.
*
* This file defines RapidJSON job deserializer implementation.
*
* Main responsibilities:
* - deserialize serialized job payloads
* - map JSON data to domain model
* - report parse errors as expected values
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include "SkeletonizationWorker/application/interfaces/job_deserializer.hpp"

namespace worker::infrastructure
{
	/**
	 * @class rapidjson_job_deserializer
	 * @brief Implements job deserialization using RapidJSON.
	 */
	class rapidjson_job_deserializer final : public application::interfaces::i_job_deserializer
	{
	public:
		/**
		 * @brief Deserializes job payload JSON.
		 *
		 * @param payload Serialized job payload.
		 * @return Parsed job model or error message.
		 */
		std::expected<job::job, std::string> deserialize(const std::string& payload) override;
	};
}
