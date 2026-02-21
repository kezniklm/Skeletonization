#pragma once

#include <string>

#include "cli_exception.hpp"

namespace cli::exceptions
{
	/**
	 * @brief Exception thrown when benchmark operations fail.
	 */
	class benchmark_exception : public cli_exception
	{
	public:
		explicit benchmark_exception(const std::string& message)
			: cli_exception(message)
		{
		}

		explicit benchmark_exception(const char* message)
			: cli_exception(message)
		{
		}
	};

	/**
	 * @brief Exception thrown when a skeletonizer algorithm fails during benchmarking.
	 */
	class skeletonizer_execution_exception final : public benchmark_exception
	{
	public:
		skeletonizer_execution_exception(const std::string& algorithm_name,
		                                 const std::string& reason)
			: benchmark_exception("Skeletonizer '" + algorithm_name + "' failed: " + reason)
			  , algorithm_name_(algorithm_name)
			  , reason_(reason)
		{
		}

		[[nodiscard]] const std::string& algorithm_name() const noexcept
		{
			return algorithm_name_;
		}

		[[nodiscard]] const std::string& reason() const noexcept
		{
			return reason_;
		}

	private:
		std::string algorithm_name_;
		std::string reason_;
	};

	/**
	 * @brief Exception thrown when benchmark name exceeds allowed length.
	 */
	class benchmark_name_too_long_exception final : public benchmark_exception
	{
	public:
		benchmark_name_too_long_exception(const std::string& name, const std::size_t max_length)
			: benchmark_exception(
				  "Benchmark name '" + name + "' exceeds maximum length of " +
				  std::to_string(max_length) + " characters")
			  , name_(name)
			  , max_length_(max_length)
		{
		}

		[[nodiscard]] const std::string& name() const noexcept
		{
			return name_;
		}

		[[nodiscard]] std::size_t max_length() const noexcept
		{
			return max_length_;
		}

	private:
		std::string name_;
		std::size_t max_length_;
	};

	/**
	 * @brief Exception thrown when no benchmarks are registered.
	 */
	class no_benchmarks_registered_exception final : public benchmark_exception
	{
	public:
		no_benchmarks_registered_exception()
			: benchmark_exception("No benchmarks registered. Please check your configuration.")
		{
		}
	};
}
