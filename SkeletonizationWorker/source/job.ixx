module;

#include <optional>
#include <string>
#include <vector>

#include "json/value.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

export module job;


namespace job
{
	using image_path = std::string;
	using algorithm_name = std::string;

	export struct image_task
	{
		image_path path;
		algorithm_name algorithm;
		bool should_run_preprocessing;
	};

	export struct job
	{
		std::string id;
		std::vector<image_task> tasks;

		static std::optional<job> from_json_string(const std::string& json_string)
		{
			rapidjson::Document document;

			const rapidjson::ParseResult parse_result = document.Parse(json_string.data(), json_string.size());

			if (!parse_result || !document.IsObject())
			{
				return std::nullopt;
			}

			job job;

			if (document.HasMember("id") && document["id"].IsString())
			{
				job.id = document["id"].GetString();
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

					image_task task;

					if (task_obj.HasMember("image_path") && task_obj["image_path"].IsString())
					{
						task.path = task_obj["image_path"].GetString();
					}

					if (task_obj.HasMember("algorithm") && task_obj["algorithm"].IsString())
					{
						task.algorithm = task_obj["algorithm"].GetString();
					}

					if (task_obj.HasMember("should_run_preprocessing") && task_obj["should_run_preprocessing"].IsBool())
					{
						task.should_run_preprocessing = task_obj["should_run_preprocessing"].GetBool();
					}
					else
					{
						task.should_run_preprocessing = false;
					}

					job.tasks.push_back(std::move(task));
				}
			}

			return job;
		}

		std::string to_json_string() const
		{
			rapidjson::Document document;

			document.SetObject();

			auto& allocator = document.GetAllocator();

			if (!id.empty())
			{
				document.AddMember(
					"id", rapidjson::Value(id.c_str(), static_cast<rapidjson::SizeType>(id.size()), allocator),
					allocator);
			}

			if (!tasks.empty())
			{
				rapidjson::Value tasks_array(rapidjson::kArrayType);

				for (const auto& [path, algorithm, should_run_preprocessing] : tasks)
				{
					rapidjson::Value task_obj(rapidjson::kObjectType);

					task_obj.AddMember(
						"image_path",
						rapidjson::Value(path.c_str(), static_cast<rapidjson::SizeType>(path.size()), allocator),
						allocator);

					task_obj.AddMember(
						"algorithm",
						rapidjson::Value(algorithm.c_str(), static_cast<rapidjson::SizeType>(algorithm.size()),
						                 allocator),
						allocator);

					task_obj.AddMember("should_run_preprocessing", should_run_preprocessing, allocator);

					tasks_array.PushBack(task_obj, allocator);
				}

				document.AddMember("tasks", tasks_array, allocator);
			}

			rapidjson::StringBuffer string_buffer;

			rapidjson::Writer writer(string_buffer);

			document.Accept(writer);

			return string_buffer.GetString();
		}
	};
}
