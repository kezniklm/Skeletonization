/**
*
* @file helpers.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares helper utilities for benchmark workflows.
*
* This file defines utility types and helper functions for parsing and
* transforming benchmark output and composing reporters.
*
* Main responsibilities:
* - parse benchmark time units and output payloads
* - convert benchmark time values to milliseconds
* - compose multiple benchmark reporters
*
* @version 1.0
* @date 2026-03-16
*/

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
	/**
	 * @brief Enumerates supported benchmark time units.
	 */
	enum class time_unit { ns, us, ms, s, unknown };

	/**
	 * @brief Parses time unit token into enum value.
	 *
	 * @param time_unit_string Time unit token.
	 * @return Parsed time unit enum value.
	 */
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

	/**
	 * @brief Converts value with unit to milliseconds.
	 *
	 * @param value Input value.
	 * @param unit Time unit token.
	 * @return Value converted to milliseconds.
	 */
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

	/**
	 * @brief Removes benchmark iterations suffix in place.
	 *
	 * @param name Benchmark name string.
	 */
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

	/// Map from benchmark names to parsed image metrics.
	using metrics_map = std::map<std::string, visual_inspector::image_metrics>;

	/**
	 * @brief Parses Google Benchmark JSON output into metrics map.
	 *
	 * @param json_text Serialized benchmark JSON text.
	 * @return Parsed metrics map.
	 */
	[[nodiscard]] metrics_map parse_google_benchmark_output(std::string_view json_text);

	/**
	 * @class composite_reporter
	 * @brief Forwards benchmark reporting callbacks to multiple reporters.
	 *
	 * @tparam Reporter Reporter types derived from BenchmarkReporter.
	 */
	template <class... Reporter>
	class composite_reporter final : public benchmark::BenchmarkReporter
	{
		static_assert(sizeof...(Reporter) >= 1,
		              "CompositeReporter needs at least one reporter.");
		static_assert((std::is_base_of_v<BenchmarkReporter, Reporter> && ...),
		              "All types must derive from benchmark::BenchmarkReporter");

	public:
		/**
		 * @brief Constructs a composite reporter.
		 *
		 * @param reporters Reporter instances to compose.
		 */
		explicit composite_reporter(Reporter&... reporters)
			: reporters_(reporters...)
		{
		}

		/**
		 * @brief Reports benchmark context to all reporters.
		 *
		 * @param c Benchmark context.
		 * @return True when all reporters accept context.
		 */
		bool ReportContext(const Context& c) override
		{
			return std::apply(
				[&](auto&... r) { return (r.ReportContext(c) && ...); },
				reporters_);
		}

		/**
		 * @brief Reports benchmark runs to all reporters.
		 *
		 * @param runs Benchmark run list.
		 */
		void ReportRuns(const std::vector<Run>& runs) override
		{
			std::apply(
				[&](auto&... r) { (r.ReportRuns(runs), ...); },
				reporters_);
		}

		/**
		 * @brief Finalizes all composed reporters.
		 */
		void Finalize() override
		{
			std::apply(
				[&](auto&... r) { (r.Finalize(), ...); },
				reporters_);
		}

	private:
		/// Tuple of reporter references.
		std::tuple<Reporter&...> reporters_;
	};
}
