#include "SkeletonizationWorker/infrastructure/rapidjson_job_deserializer.hpp"

#include <string>
#include <string_view>

#include "rapidjson/document.h"

#include "SkeletonizationWorker/domain/output_format.hpp"

namespace worker::infrastructure
{
	std::expected<job::job, std::string> rapidjson_job_deserializer::deserialize(const std::string& payload)
	{
		rapidjson::Document document;

		const rapidjson::ParseResult parse_result = document.Parse(payload.data(), payload.size());

		if (!parse_result || !document.IsObject())
		{
			return std::unexpected("Invalid JSON payload");
		}

		job::job job;

		if (document.HasMember("id") && document["id"].IsString())
		{
			job.id = document["id"].GetString();
		}
		else if (document.HasMember("job_id") && document["job_id"].IsString())
		{
			job.id = document["job_id"].GetString();
		}
		else
		{
			return std::unexpected("Missing or invalid job id (expected 'id' or 'job_id')");
		}

		if (!document.HasMember("tasks") || !document["tasks"].IsArray() || document["tasks"].Empty())
		{
			return std::unexpected("Job contains no tasks");
		}

		const auto& tasks_array = document["tasks"];

		job.tasks.reserve(tasks_array.Size());

		for (rapidjson::SizeType i = 0; i < tasks_array.Size(); ++i)
		{
			if (!tasks_array[i].IsObject())
			{
				return std::unexpected("Invalid task (expected object) at index " + std::to_string(i));
			}

			const auto& task_json_object = tasks_array[i];

			job::image_task task{};

			if (task_json_object.HasMember("image_key") && task_json_object["image_key"].IsString())
			{
				task.path = task_json_object["image_key"].GetString();
			}
			else if (task_json_object.HasMember("image_path") && task_json_object["image_path"].IsString())
			{
				task.path = task_json_object["image_path"].GetString();
			}
			else
			{
				return std::unexpected("Missing or invalid task.image_key/image_path at index " + std::to_string(i));
			}

			if (!task_json_object.HasMember("algorithm") || !task_json_object["algorithm"].IsString())
			{
				return std::unexpected("Missing or invalid task.algorithm at index " + std::to_string(i));
			}
			task.algorithm = task_json_object["algorithm"].GetString();

			if (!task_json_object.HasMember("should_run_preprocessing") || !task_json_object["should_run_preprocessing"]
				.IsBool())
			{
				return std::unexpected(
					"Missing or invalid task.should_run_preprocessing at index " + std::to_string(i));
			}

			task.should_run_preprocessing = task_json_object["should_run_preprocessing"].GetBool();

			if (!task_json_object.HasMember("output_format") || !task_json_object["output_format"].IsString())
			{
				return std::unexpected("Missing or invalid task.output_format at index " + std::to_string(i));
			}

			const auto parse_output_format_result = job::parse_output_format(
				task_json_object["output_format"].GetString());

			if (!parse_output_format_result)
			{
				return std::unexpected("Invalid output format at index " + std::to_string(i));
			}

			task.format = parse_output_format_result.value();

			job.tasks.push_back(std::move(task));
		}

		return job;
	}
}
