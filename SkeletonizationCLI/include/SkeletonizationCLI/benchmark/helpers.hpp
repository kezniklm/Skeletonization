#pragma once

#include <map>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <vector>

#include "benchmark/benchmark.h"
#include "SkeletonizationCLI/visual_inspector/visualiser.hpp"

namespace skeletonization_benchmark
{
	enum class time_unit { ns, us, ms, s, unknown };

	constexpr time_unit parse_time_unit(std::string_view time_unit_string) noexcept
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

	[[nodiscard]] inline double to_milliseconds(double value,
	                                            std::string_view unit) noexcept
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

	[[nodiscard]] metrics_map parse_google_benchmark_output(std::string_view json_text);

	template <class... Reporter>
	class composite_reporter final : public benchmark::BenchmarkReporter
	{
		static_assert(sizeof...(Reporter) >= 1,
		              "CompositeReporter needs at least one reporter.");
		static_assert((std::is_base_of_v<BenchmarkReporter, Reporter> && ...),
		              "All types must derive from benchmark::BenchmarkReporter");

	public:
		explicit composite_reporter(Reporter&... reporters)
			: reporters_(reporters...)
		{
		}

		bool ReportContext(const Context& c) override
		{
			return std::apply(
				[&](auto&... r) { return (r.ReportContext(c) && ...); },
				reporters_);
		}

		void ReportRuns(const std::vector<Run>& runs) override
		{
			std::apply(
				[&](auto&... r) { (r.ReportRuns(runs), ...); },
				reporters_);
		}

		void Finalize() override
		{
			std::apply(
				[&](auto&... r) { (r.Finalize(), ...); },
				reporters_);
		}

	private:
		std::tuple<Reporter&...> reporters_;
	};
}
