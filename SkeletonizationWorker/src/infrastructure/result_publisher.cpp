/**
*
* @file result_publisher.cpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Implements result publishing workflows.
*
* This file implements serialization and publishing of task result payloads.
*
* Main responsibilities:
* - compose task result payload documents
* - publish serialized payloads via transport
* - include worker metadata and timing values
*
* @version 1.0
* @date 2026-03-16
*/

#include <glog/logging.h>

#include "SkeletonizationWorker/infrastructure/result_publisher.hpp"

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

namespace worker::infrastructure
{
	result_publisher::result_publisher(application::interfaces::i_result_transport& result_sink,
	                                   configuration::dependency_injection::worker_id_t worker_id,
	                                   skeletonizer::skeletonizer_type processor_type)
		: result_sink_(result_sink),
		  worker_id_(std::move(worker_id.value))
	{
		const auto type_view = skeletonizer::to_string_view(processor_type);
		if (type_view == "thread")
		{
			worker_type_ = "mt";
		}
		else
		{
			worker_type_ = std::string{type_view};
		}
	}

	std::expected<void, std::string> result_publisher::publish_task_result(
		const std::string& job_id,
		const std::string& image_path,
		const std::string& algorithm,
		const std::string& output_path,
		const bool success,
		const double processing_time_ms,
		const std::string& error_message)
	{
		rapidjson::Document document;
		document.SetObject();

		auto& allocator = document.GetAllocator();

		document.AddMember(
			"job_id", rapidjson::Value(job_id.c_str(), static_cast<rapidjson::SizeType>(job_id.size()), allocator),
			allocator);
		document.AddMember("worker_id",
		                   rapidjson::Value(worker_id_.c_str(), static_cast<rapidjson::SizeType>(worker_id_.size()),
		                                    allocator),
		                   allocator);
		document.AddMember("worker_type",
		                   rapidjson::Value(worker_type_.c_str(), static_cast<rapidjson::SizeType>(worker_type_.size()),
		                                    allocator),
		                   allocator);
		document.AddMember("image_path",
		                   rapidjson::Value(image_path.c_str(), static_cast<rapidjson::SizeType>(image_path.size()),
		                                    allocator), allocator);
		document.AddMember("algorithm",
		                   rapidjson::Value(algorithm.c_str(), static_cast<rapidjson::SizeType>(algorithm.size()),
		                                    allocator), allocator);
		document.AddMember("output_path",
		                   rapidjson::Value(output_path.c_str(), static_cast<rapidjson::SizeType>(output_path.size()),
		                                    allocator), allocator);
		document.AddMember("success", success, allocator);
		document.AddMember("processing_time_ms", processing_time_ms, allocator);

		if (!success && !error_message.empty())
		{
			document.AddMember("error", rapidjson::Value(error_message.c_str(),
			                                             static_cast<rapidjson::SizeType>(error_message.size()),
			                                             allocator), allocator);
			document.AddMember("error_message", rapidjson::Value(error_message.c_str(),
			                                                     static_cast<rapidjson::SizeType>(error_message.size()),
			                                                     allocator), allocator);
		}

		rapidjson::StringBuffer buffer;
		rapidjson::Writer writer(buffer);
		document.Accept(writer);

		const std::string json_result = buffer.GetString();

		const auto publish_result = result_sink_.publish_result(json_result);

		if (!publish_result)
		{
			return std::unexpected("Failed to publish result: " + publish_result.error());
		}

		LOG(INFO) << "Published result for job " << job_id << " (success=" << success << ")";

		return {};
	}
}
