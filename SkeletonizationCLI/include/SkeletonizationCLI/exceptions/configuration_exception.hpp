#pragma once

#include <filesystem>
#include <string>

#include "cli_exception.hpp"

namespace cli::exceptions
{
	/**
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

		[[nodiscard]] const std::filesystem::path& path() const noexcept
		{
			return path_;
		}

	private:
		std::filesystem::path path_;
	};

	/**
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

		[[nodiscard]] const std::string& filename() const noexcept
		{
			return filename_;
		}

	private:
		std::string filename_;
	};

	/**
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
