/**
*
* @file configuration_exception.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares configuration-related exception type.
*
* This file defines configuration exception hierarchy for file loading,
* parsing, and validation failures.
*
* Main responsibilities:
* - define base configuration exception type
* - define file-not-found and parse exceptions
* - define configuration validation exception
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <filesystem>
#include <string>

#include "cli_exception.hpp"

namespace cli::exceptions
{
	/**
	 * @class configuration_exception
	 * @brief Exception thrown when configuration file operations fail.
	 */
	class configuration_exception : public cli_exception
	{
	public:
		explicit configuration_exception(const std::string& message)
			: cli_exception(message)
		{
		}

		explicit configuration_exception(const char* message)
			: cli_exception(message)
		{
		}
	};

	/**
	 * @class configuration_file_not_found_exception
	 * @brief Exception thrown when a configuration file cannot be opened.
	 */
	class configuration_file_not_found_exception final : public configuration_exception
	{
	public:
		explicit configuration_file_not_found_exception(const std::filesystem::path& path)
			: configuration_exception("Cannot open configuration file: " + path.string())
			  , path_(path)
		{
		}

		/**
		 * @brief Returns missing file path.
		 *
		 * @return Missing configuration path.
		 */
		[[nodiscard]] const std::filesystem::path& path() const noexcept
		{
			return path_;
		}

	private:
		/// Missing configuration path.
		std::filesystem::path path_;
	};

	/**
	 * @class configuration_parse_exception
	 * @brief Exception thrown when JSON parsing fails.
	 */
	class configuration_parse_exception final : public configuration_exception
	{
	public:
		explicit configuration_parse_exception(const std::string& filename,
		                                       const std::string& details = "")
			: configuration_exception(
				  "Failed to parse JSON configuration file: " + filename +
				  (details.empty() ? "" : " - " + details))
			  , filename_(filename)
		{
		}

		/**
		 * @brief Returns filename that failed to parse.
		 *
		 * @return Configuration filename.
		 */
		[[nodiscard]] const std::string& filename() const noexcept
		{
			return filename_;
		}

	private:
		/// Configuration filename.
		std::string filename_;
	};

	/**
	 * @class configuration_validation_exception
	 * @brief Exception thrown when configuration structure is invalid.
	 */
	class configuration_validation_exception final : public configuration_exception
	{
	public:
		explicit configuration_validation_exception(const std::string& message)
			: configuration_exception(message)
		{
		}
	};
}
