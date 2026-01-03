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
	class job_processor final : public interfaces::i_job_processor
	{
	public:
		job_processor(
			interfaces::i_image_service& image_service,
			interfaces::i_object_storage& object_storage,
			interfaces::i_skeletonization_processor& processor,
			interfaces::i_result_publisher& publisher,
			configuration::dependency_injection::output_directory_t output_directory);

		std::expected<void, std::string> process_job(const job::job& job_data) override;

	private:
		interfaces::i_image_service& image_service_;
		interfaces::i_object_storage& object_storage_;
		interfaces::i_skeletonization_processor& skeletonization_processor_;
		interfaces::i_result_publisher& result_publisher_;
		std::string output_directory_;

		std::expected<void, std::string> process_task(const std::string& job_id, const job::image_task& task) const;
	};
}
