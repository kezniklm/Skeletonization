#pragma once

#include <optional>
#include <string_view>

#include "SkeletonizationCore/extensions/string.hpp"

namespace job
{
	enum class output_format
	{
		png,
		jpeg,
		bmp,
		tiff
	};

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
