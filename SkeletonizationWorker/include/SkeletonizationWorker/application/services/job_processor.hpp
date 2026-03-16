/**
*
* @file job_processor.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares the worker job processor service.
*
* This file defines the application job processor implementation that
* orchestrates image loading, processing, and result publishing.
*
* Main responsibilities:
* - orchestrate processing of job tasks
* - coordinate storage and image services
* - publish task processing results
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <expected>
#include <string>

#include "SkeletonizationWorker/configuration.hpp"
#include "SkeletonizationWorker/domain/job.hpp"

#include "SkeletonizationWorker/application/interfaces/image_service.hpp"
#include "SkeletonizationWorker/application/interfaces/job_processor.hpp"
#include "SkeletonizationWorker/application/interfaces/object_storage.hpp"
#include "SkeletonizationWorker/application/interfaces/result_publisher.hpp"
#include "SkeletonizationWorker/application/interfaces/skeletonization_processor.hpp"

namespace worker::application::services
{
	/**
	 * @class job_processor
	 * @brief Implements job processing orchestration workflow.
	 */
	class job_processor final : public interfaces::i_job_processor
	{
	public:
		/**
		 * @brief Constructs job processor with required dependencies.
		 * @param image_service Image load/save service.
		 * @param object_storage Object storage backend service.
		 * @param processor Skeletonization processing service.
		 * @param publisher Result publishing service.
		 * @param output_directory Output directory DI wrapper.
		 */
		job_processor(
			interfaces::i_image_service& image_service,
			interfaces::i_object_storage& object_storage,
			interfaces::i_skeletonization_processor& processor,
			interfaces::i_result_publisher& publisher,
			configuration::dependency_injection::output_directory_t output_directory);

		/**
		 * @brief Processes a full job payload.
		 *
		 * @param job_data Deserialized job model.
		 * @return Empty result on success or error message.
		 */
		std::expected<void, std::string> process_job(const job::job& job_data) override;

	private:
		/// Image service dependency.
		interfaces::i_image_service& image_service_;
		/// Object storage dependency.
		interfaces::i_object_storage& object_storage_;
		/// Skeletonization processor dependency.
		interfaces::i_skeletonization_processor& skeletonization_processor_;
		/// Result publisher dependency.
		interfaces::i_result_publisher& result_publisher_;
		/// Output directory path.
		std::string output_directory_;

		/**
		 * @brief Processes one image task from a job.
		 *
		 * @param job_id Parent job identifier.
		 * @param task Image task payload.
		 * @return Empty result on success or error message.
		 */
		std::expected<void, std::string> process_task(const std::string& job_id, const job::image_task& task) const;
	};
}
