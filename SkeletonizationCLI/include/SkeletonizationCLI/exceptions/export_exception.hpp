#pragma once

#include <filesystem>
#include <string>

#include "cli_exception.hpp"

namespace cli::exceptions
{
	/**
	 * @brief Exception thrown when export operations fail.
	 */
	class export_exception : public cli_exception
	{
	public:
		explicit export_exception(const std::string& message)
			: cli_exception(message)
		{
		}

		explicit export_exception(const char* message)
			: cli_exception(message)
		{
		}
	};

	/**
	 * @brief Exception thrown when JSON export fails.
	 */
	class json_export_exception final : public export_exception
	{
	public:
		explicit json_export_exception(const std::filesystem::path& path,
		                               const std::string& reason = "")
			: export_exception(
				  "Failed to export JSON to: " + path.string() +
				  (reason.empty() ? "" : " - " + reason))
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
	 * @brief Exception thrown when image encoding fails.
	 */
	class image_encoding_exception final : public export_exception
	{
	public:
		explicit image_encoding_exception(const std::string& format)
			: export_exception("Failed to encode image to format: " + format)
			  , format_(format)
		{
		}

		[[nodiscard]] const std::string& format() const noexcept
		{
			return format_;
		}

	private:
		std::string format_;
	};
}
