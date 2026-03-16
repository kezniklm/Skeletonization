/**
*
* @file result_publisher.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares the result publisher contract.
 *
 * This file defines interface for publishing task-level processing results.
 *
 * Main responsibilities:
 * - define task result publishing contract
 * - capture job and algorithm context in result payload
 * - surface publishing failures as expected values
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
	 * @class i_result_publisher
	 * @brief Defines publishing operations for task-level processing results.
	 */
	class i_result_publisher
	{
	public:
		/**
		 * @brief Destroys the result publisher interface.
		 */
		virtual ~i_result_publisher() = default;

		/**
		 * @brief Publishes one task processing result.
		 *
		 * @param job_id Parent job identifier.
		 * @param image_path Source image path.
		 * @param algorithm Algorithm name.
		 * @param output_path Produced output path.
		 * @param success Indicates whether processing succeeded.
		 * @param processing_time_ms Processing duration in milliseconds.
		 * @param error_message Optional error message when processing fails.
		 * @return Empty result on success or error message.
		 */
		virtual std::expected<void, std::string> publish_task_result(
			const std::string& job_id,
			const std::string& image_path,
			const std::string& algorithm,
			const std::string& output_path,
			bool success,
			double processing_time_ms,
			const std::string& error_message = "") = 0;
	};
}
