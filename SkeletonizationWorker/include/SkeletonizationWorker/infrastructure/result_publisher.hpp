#pragma once

#include <expected>
#include <string>

#include "SkeletonizationWorker/application/interfaces/result_publisher.hpp"
#include "SkeletonizationWorker/application/interfaces/result_transport.hpp"

namespace worker::infrastructure
{
	class result_publisher final : public application::interfaces::i_result_publisher
	{
	public:
		explicit result_publisher(application::interfaces::i_result_transport& result_sink);

		std::expected<void, std::string> publish_task_result(
			const std::string& job_id,
			const std::string& image_path,
			const std::string& algorithm,
			const std::string& output_path,
			bool success,
			double processing_time_ms,
			const std::string& error_message = "") override;

	private:
		application::interfaces::i_result_transport& result_sink_;
	};
}
