/**
*
* @file output_format.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares output format types used by the worker.
*
* This file defines output format enum values and helper conversion
* functions for extensions, MIME types, and parsing.
*
* Main responsibilities:
* - define supported output formats
* - map formats to extension and MIME strings
* - parse string values into format enum
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <optional>
#include <string_view>

#include "SkeletonizationCore/extensions/string.hpp"

namespace job
{
	/**
	 * @brief Enumerates supported output image formats.
	 */
	enum class output_format
	{
		png,
		jpeg,
		bmp,
		tiff
	};

	/**
	 * @brief Returns file extension for output format.
	 *
	 * @param format Output format value.
	 * @return File extension without leading dot.
	 */
	constexpr std::string_view to_extension(const output_format format) noexcept
	{
		switch (format)
		{
		case output_format::png: return "png";
		case output_format::jpeg: return "jpg";
		case output_format::bmp: return "bmp";
		case output_format::tiff: return "tiff";
		default: return "png";
		}
	}

	/**
	 * @brief Returns MIME type for output format.
	 *
	 * @param format Output format value.
	 * @return MIME type string.
	 */
	constexpr std::string_view to_mime_type(const output_format format) noexcept
	{
		switch (format)
		{
		case output_format::png: return "image/png";
		case output_format::jpeg: return "image/jpeg";
		case output_format::bmp: return "image/bmp";
		case output_format::tiff: return "image/tiff";
		default: return "image/png";
		}
	}

	/**
	 * @brief Parses output format token.
	 *
	 * @param format Format token string.
	 * @return Parsed format value when token is valid.
	 */
	constexpr std::optional<output_format> parse_output_format(const std::string_view format) noexcept
	{
		if (equals_ascii(format, "PNG"))
		{
			return output_format::png;
		}

		if (equals_ascii(format, "JPEG") || equals_ascii(format, "JPG"))
		{
			return output_format::jpeg;
		}

		if (equals_ascii(format, "BMP"))
		{
			return output_format::bmp;
		}

		if (equals_ascii(format, "TIFF") || equals_ascii(format, "TIF"))
		{
			return output_format::tiff;
		}

		return std::nullopt;
	}
}
