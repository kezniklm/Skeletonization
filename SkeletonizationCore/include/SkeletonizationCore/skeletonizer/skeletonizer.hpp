#pragma once

#include <string>
#include <optional>

#include "opencv2/core.hpp"

namespace skeletonizer
{
	template <int ForegroundValue = 1, int SkeletonValue = 1, int BackgroundValue = 0>
	class skeletonizer
	{
	public:
		virtual ~skeletonizer() = default;

		virtual void apply(cv::Mat& binary_image) const = 0;

		virtual std::string name() const = 0;

	protected:
		static constexpr int foreground = ForegroundValue;
		static constexpr int background = BackgroundValue;
		static constexpr int skeleton = SkeletonValue;

		virtual bool has_changed(cv::InputArray& difference) const
		{
			return cv::countNonZero(difference) > 0;
		}

		static void clear_border(const cv::Mat& binary_image)
		{
			binary_image.row(0).setTo(0);
			binary_image.row(binary_image.rows - 1).setTo(0);
			binary_image.col(0).setTo(0);
			binary_image.col(binary_image.cols - 1).setTo(0);
		}
	};

	enum class skeletonizer_type
	{
		cpu,
		thread,
		gpu
	};

	[[nodiscard]] constexpr std::string_view to_string_view(const skeletonizer_type type) noexcept
	{
		using enum skeletonizer_type;

		switch (type)
		{
		case cpu: return "cpu";
		case thread: return "thread";
		case gpu: return "gpu";
		default: return "unknown";
		}
	}

	[[nodiscard]] inline std::string to_string(const skeletonizer_type type, const bool to_upper = true)
	{
		std::string s{to_string_view(type)};

		if (to_upper)
		{
			std::ranges::transform(s, s.begin(), [](const unsigned char c)
			{
				return static_cast<char>(std::toupper(c));
			});
		}

		return s;
	}

	inline [[nodiscard]] std::optional<skeletonizer_type> from_string(const std::string_view value)
	{
		if (value == "gpu")
		{
			return skeletonizer_type::gpu;
		}
		if (value == "thread")
		{
			return skeletonizer_type::thread;
		}
		if (value == "cpu")
		{
			return skeletonizer_type::cpu;
		}

		return std::nullopt;
	}
}
