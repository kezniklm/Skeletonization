/**
*
* @file benchmark_exception.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares benchmark-specific exception type.
*
* This file defines typed benchmark exception hierarchy used by benchmark
* execution and registration workflows.
*
* Main responsibilities:
* - define benchmark exception base type
* - define algorithm execution failure exception
* - define benchmark naming and registration exceptions
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <string>

#include "cli_exception.hpp"

namespace cli::exceptions
{
	/**
	 * @class benchmark_exception
	 * @brief Exception thrown when benchmark operations fail.
	 */
	class benchmark_exception : public cli_exception
	{
	public:
		/**
		 * @brief Constructs benchmark exception from message string.
		 *
		 * @param message Error message.
		 */
		explicit benchmark_exception(const std::string& message)
			: cli_exception(message)
		{
		}

		/**
		 * @brief Constructs benchmark exception from C-string message.
		 *
		 * @param message Error message.
		 */
		explicit benchmark_exception(const char* message)
			: cli_exception(message)
		{
		}
	};

	/**
	 * @class skeletonizer_execution_exception
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

		/**
		 * @brief Returns algorithm name that failed.
		 *
		 * @return Algorithm name.
		 */
		[[nodiscard]] const std::string& algorithm_name() const noexcept
		{
			return algorithm_name_;
		}

		/**
		 * @brief Returns failure reason.
		 *
		 * @return Failure reason text.
		 */
		[[nodiscard]] const std::string& reason() const noexcept
		{
			return reason_;
		}

	private:
		/// Failed algorithm name.
		std::string algorithm_name_;
		/// Failure reason.
		std::string reason_;
	};

	/**
	 * @class benchmark_name_too_long_exception
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

		/**
		 * @brief Returns benchmark name that exceeded limit.
		 *
		 * @return Benchmark name.
		 */
		[[nodiscard]] const std::string& name() const noexcept
		{
			return name_;
		}

		/**
		 * @brief Returns maximum permitted length.
		 *
		 * @return Maximum name length.
		 */
		[[nodiscard]] std::size_t max_length() const noexcept
		{
			return max_length_;
		}

	private:
		/// Benchmark name value.
		std::string name_;
		/// Maximum allowed length.
		std::size_t max_length_;
	};

	/**
	 * @class no_benchmarks_registered_exception
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
