#include "SkeletonizationWorker/infrastructure/rapidjson_job_deserializer.hpp"

#include "rapidjson/document.h"

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

		if (document.HasMember("tasks") && document["tasks"].IsArray())
		{
			const auto& tasks_array = document["tasks"];
			job.tasks.reserve(tasks_array.Size());

			for (rapidjson::SizeType i = 0; i < tasks_array.Size(); ++i)
			{
				if (!tasks_array[i].IsObject())
				{
					continue;
				}

				const auto& task_obj = tasks_array[i];
				job::image_task task{};

				if (task_obj.HasMember("image_key") && task_obj["image_key"].IsString())
				{
					task.path = task_obj["image_key"].GetString();
				}
				else if (task_obj.HasMember("image_path") && task_obj["image_path"].IsString())
				{
					task.path = task_obj["image_path"].GetString();
				}
				else
				{
					continue;
				}

				if (!task_obj.HasMember("algorithm") || !task_obj["algorithm"].IsString())
				{
					continue;
				}
				task.algorithm = task_obj["algorithm"].GetString();

				if (task_obj.HasMember("should_run_preprocessing") && task_obj["should_run_preprocessing"].IsBool())
				{
					task.should_run_preprocessing = task_obj["should_run_preprocessing"].GetBool();
				}

				job.tasks.push_back(std::move(task));
			}

			if (tasks_array.Size() > 0 && job.tasks.empty())
			{
				return std::unexpected("Job contains no valid tasks (expected tasks with 'image_key' or 'image_path' and 'algorithm')");
			}
		}

		return job;
	}
}
