/**
*
* @file export_exception.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares export-related exception type.
*
* This file defines export exception hierarchy used for JSON export,
* image loading, and image encoding failures.
*
* Main responsibilities:
* - define base export exception type
* - define JSON export and image loading exceptions
* - define image encoding exception type
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
	 * @class export_exception
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
	 * @class json_export_exception
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

		/**
		 * @brief Returns output path that failed export.
		 *
		 * @return Output path.
		 */
		[[nodiscard]] const std::filesystem::path& path() const noexcept
		{
			return path_;
		}

	private:
		/// Output path for failed JSON export.
		std::filesystem::path path_;
	};

	/**
	 * @class image_loading_exception
	 * @brief Exception thrown when image loading fails.
	 */
	class image_loading_exception final : public cli_exception
	{
	public:
		explicit image_loading_exception(const std::filesystem::path& path,
		                                 const std::string& reason = "")
			: cli_exception(
				  "Failed to load image from: " + path.string() +
				  (reason.empty() ? "" : " - " + reason))
			  , path_(path)
		{
		}

		/**
		 * @brief Returns image path that failed loading.
		 *
		 * @return Image path.
		 */
		[[nodiscard]] const std::filesystem::path& path() const noexcept
		{
			return path_;
		}

	private:
		/// Image path that failed loading.
		std::filesystem::path path_;
	};

	/**
	 * @class image_encoding_exception
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

		/**
		 * @brief Returns failed encoding format.
		 *
		 * @return Encoding format token.
		 */
		[[nodiscard]] const std::string& format() const noexcept
		{
			return format_;
		}

	private:
		/// Failed encoding format token.
		std::string format_;
	};
}
