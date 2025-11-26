module;

#include <map>
#include <string>

#include "benchmark/benchmark.h"
#include "rapidjson/document.h"

export module benchmark:helpers;

import visual_inspector;

namespace skeletonization_benchmark
{
	enum class time_unit { ns, us, ms, s, unknown };

	constexpr time_unit parse_time_unit(const std::string_view time_unit_string) noexcept
	{
		if (time_unit_string.size() == 1 && time_unit_string[0] == 's')
		{
			return time_unit::s;
		}

		if (time_unit_string.size() != 2 || time_unit_string[1] != 's')
		{
			return time_unit::unknown;
		}

		switch (time_unit_string[0])
		{
		case 'n': return time_unit::ns;
		case 'u': return time_unit::us;
		case 'm': return time_unit::ms;
		default: return time_unit::unknown;
		}
	}

	[[nodiscard]] inline double to_milliseconds(const double value, const std::string_view unit) noexcept
	{
		switch (parse_time_unit(unit))
		{
		case time_unit::ns: return value / 1'000'000.0;
		case time_unit::us: return value / 1'000.0;
		case time_unit::ms: return value;
		case time_unit::s: return value * 1'000.0;
		default: return value;
		}
	}

	inline void strip_iterations_suffix_inplace(std::string& name) noexcept
	{
		constexpr std::string_view token = "/iterations:";

		const auto pos = name.find(token);

		if (pos == std::string::npos)
		{
			return;
		}

		name.erase(pos);
	}

	using metrics_map = std::map<std::string, visual_inspector::image_metrics>;

	[[nodiscard]] metrics_map parse_google_benchmark_output(const std::string_view json_text)
	{
		metrics_map metrics_by_name;

		rapidjson::Document document;

		document.Parse(json_text.data(), static_cast<rapidjson::SizeType>(json_text.size()));

		if (document.HasParseError() || !document.IsObject())
		{
			return metrics_by_name;
		}

		constexpr auto benchmarks_section_name = "benchmarks";

		if (!document.HasMember(benchmarks_section_name) || !document[benchmarks_section_name].IsArray())
		{
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

			if (!object.HasMember(json_name_section) || !object[json_name_section].IsString())
			{
				continue;
			}

			std::string benchmark_name{
				object[json_name_section].GetString(), object[json_name_section].GetStringLength()
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

			const auto read_real_time = [&](const char* key, double& out)
			{
				if (object.HasMember(key) && object[key].IsNumber())
				{
					out = to_milliseconds(metrics.real_time, metrics.time_unit);
				}
			};

			read_int64(json_iterations_section, metrics.iterations);
			read_time_unit(json_time_unit_section, metrics.time_unit);
			read_double(json_real_time_section, metrics.real_time);
			read_double(json_cpu_time_section, metrics.cpu_time);
			read_double(json_bytes_per_second_section, metrics.bytes_per_second);
			read_double(json_items_per_second_section, metrics.items_per_second);
			read_real_time(json_real_time_section, metrics.execution_time_ms);

			metrics_by_name.emplace(std::move(benchmark_name), std::move(metrics));
		}

		return metrics_by_name;
	}

	export template <class... Reporter>
	class composite_reporter final : public benchmark::BenchmarkReporter
	{
		static_assert(sizeof...(Reporter) >= 1, "CompositeReporter needs at least one reporter.");
		static_assert((std::is_base_of_v<BenchmarkReporter, Reporter> && ...),
		              "All types must derive from benchmark::BenchmarkReporter");

	public:
		explicit composite_reporter(Reporter&... reporters)
			: reporters_(reporters...)
		{
		}

		bool ReportContext(const Context& c) override
		{
			return std::apply([&](auto&... r) { return (r.ReportContext(c) && ...); }, reporters_);
		}

		void ReportRuns(const std::vector<Run>& runs) override
		{
			std::apply([&](auto&... r) { (r.ReportRuns(runs), ...); }, reporters_);
		}

		void Finalize() override
		{
			std::apply([&](auto&... r) { (r.Finalize(), ...); }, reporters_);
		}

	private:
		std::tuple<Reporter&...> reporters_;
	};
}
