/**
*
* @file cli_exception.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares base CLI exception type.
*
* This file defines the root exception type for CLI module failures.
*
* Main responsibilities:
* - define base runtime exception for CLI errors
* - provide common constructor overloads
* - support copy and move exception semantics
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <stdexcept>
#include <string>

namespace cli::exceptions
{
	/**
	 * @class cli_exception
	 * @brief Base exception class for all CLI-related errors.
	 *
	 * Provides a common base for typed exceptions in the CLI module,
	 * enabling consistent error handling and categorization.
	 */
	class cli_exception : public std::runtime_error
	{
	public:
		/**
		 * @brief Constructs CLI exception from message string.
		 *
		 * @param message Error message text.
		 */
		explicit cli_exception(const std::string& message)
			: std::runtime_error(message)
		{
		}

		/**
		 * @brief Constructs CLI exception from C-string message.
		 *
		 * @param message Error message text.
		 */
		explicit cli_exception(const char* message)
			: std::runtime_error(message)
		{
		}

		/**
		 * @brief Destroys the CLI exception instance.
		 */
		~cli_exception() override = default;

		cli_exception(const cli_exception&) = default;
		cli_exception& operator=(const cli_exception&) = default;
		cli_exception(cli_exception&&) noexcept = default;
		cli_exception& operator=(cli_exception&&) noexcept = default;
	};
}
