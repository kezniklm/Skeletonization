#pragma once

#include <stdexcept>
#include <string>

namespace cli::exceptions
{
	/**
	 * @brief Base exception class for all CLI-related errors.
	 *
	 * Provides a common base for typed exceptions in the CLI module,
	 * enabling consistent error handling and categorization.
	 */
	class cli_exception : public std::runtime_error
	{
	public:
		explicit cli_exception(const std::string& message)
			: std::runtime_error(message)
		{
		}

		explicit cli_exception(const char* message)
			: std::runtime_error(message)
		{
		}

		~cli_exception() override = default;

		cli_exception(const cli_exception&) = default;
		cli_exception& operator=(const cli_exception&) = default;
		cli_exception(cli_exception&&) noexcept = default;
		cli_exception& operator=(cli_exception&&) noexcept = default;
	};
}
