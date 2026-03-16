/**
*
* @file result_publisher.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares result publishing infrastructure implementation.
*
* This file defines result publisher implementation that serializes task
* results and sends them through result transport.
*
* Main responsibilities:
* - build task result payloads
* - publish serialized results to transport
* - include worker metadata in published results
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <expected>
#include <string>

#include "SkeletonizationWorker/configuration.hpp"
#include "SkeletonizationCore/skeletonizer/skeletonizer.hpp"

#include "SkeletonizationWorker/application/interfaces/result_publisher.hpp"
#include "SkeletonizationWorker/application/interfaces/result_transport.hpp"

namespace worker::infrastructure
{
	/**
	 * @class result_publisher
	 * @brief Implements publishing of task-level processing results.
	 */
	class result_publisher final : public application::interfaces::i_result_publisher
	{
	public:
		explicit result_publisher(application::interfaces::i_result_transport& result_sink,
		                          configuration::dependency_injection::worker_id_t worker_id,
		                          skeletonizer::skeletonizer_type processor_type);

		std::expected<void, std::string> publish_task_result(
			const std::string& job_id,
			const std::string& image_path,
			const std::string& algorithm,
			const std::string& output_path,
			bool success,
			double processing_time_ms,
			const std::string& error_message = "") override;

	private:
		/// Result transport dependency.
		application::interfaces::i_result_transport& result_sink_;
		/// Worker identifier.
		std::string worker_id_;
		/// Worker processor type string.
		std::string worker_type_;
	};
}
