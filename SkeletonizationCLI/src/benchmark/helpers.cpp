#include <string_view>

#include "glog/logging.h"
#include "rapidjson/document.h"

#include "SkeletonizationCLI/benchmark/helpers.hpp"
#include "SkeletonizationCLI/visual_inspector/visualiser.hpp"

namespace skeletonization_benchmark
{
	metrics_map parse_google_benchmark_output(const std::string_view json_text)
	{
		metrics_map metrics_by_name;

		rapidjson::Document document;
		document.Parse(json_text.data(),
		               static_cast<rapidjson::SizeType>(json_text.size()));

		if (document.HasParseError() || !document.IsObject())
		{
			LOG(WARNING) << "Failed to parse benchmark JSON output (error code: "
				<< document.GetParseError() << ")";
			return metrics_by_name;
		}

		constexpr auto benchmarks_section_name = "benchmarks";

		if (!document.HasMember(benchmarks_section_name) ||
			!document[benchmarks_section_name].IsArray())
		{
			LOG(WARNING) << "No 'benchmarks' section found in benchmark JSON output.";
			return metrics_by_name;
		}

		const auto benchmarks = document[benchmarks_section_name].GetArray();

		for (const auto& entry : benchmarks)
		{
			if (!entry.IsObject())
			{
				continue;
			}

			const auto object = entry.GetObject();

			constexpr auto json_name_section = "name";
			constexpr auto json_iterations_section = "iterations";
			constexpr auto json_time_unit_section = "time_unit";
			constexpr auto json_real_time_section = "real_time";
			constexpr auto json_cpu_time_section = "cpu_time";
			constexpr auto json_bytes_per_second_section = "bytes_per_second";
			constexpr auto json_items_per_second_section = "items_per_second";

			if (!object.HasMember(json_name_section) ||
				!object[json_name_section].IsString())
			{
				continue;
			}

			std::string benchmark_name{
				object[json_name_section].GetString(),
				object[json_name_section].GetStringLength()
			};

			strip_iterations_suffix_inplace(benchmark_name);

			visual_inspector::image_metrics metrics{};

			const auto read_double = [&](const char* key, double& out)
			{
				if (object.HasMember(key) && object[key].IsNumber())
				{
					out = object[key].GetDouble();
				}
			};

			const auto read_int64 = [&](const char* key, std::int64_t& out)
			{
				if (object.HasMember(key) && object[key].IsInt64())
				{
					out = object[key].GetInt64();
				}
			};

			const auto read_time_unit = [&](const char* key, std::string& out)
			{
				if (object.HasMember(key) && object[key].IsString())
				{
					out = object[key].GetString();
				}
			};

			read_int64(json_iterations_section, metrics.iterations);
			read_time_unit(json_time_unit_section, metrics.time_unit);
			read_double(json_real_time_section, metrics.real_time);
			read_double(json_cpu_time_section, metrics.cpu_time);
			read_double(json_bytes_per_second_section, metrics.bytes_per_second);
			read_double(json_items_per_second_section, metrics.items_per_second);

			// Convert real_time to milliseconds for consistent display
			metrics.execution_time_ms = to_milliseconds(metrics.real_time, metrics.time_unit);

			metrics_by_name.emplace(std::move(benchmark_name),
			                        std::move(metrics));
		}

		return metrics_by_name;
	}
}
