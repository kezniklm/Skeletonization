#pragma once

#include <expected>
#include <string>

namespace worker::application::interfaces
{
	class i_result_publisher
	{
	public:
		virtual ~i_result_publisher() = default;

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
