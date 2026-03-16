/**
*
* @file skeletonizer.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Defines the base skeletonizer interface and skeletonizer type utilities.
*
* This header provides the abstract base class for binary-image skeletonization
* algorithms and utility conversions for execution backend types. It centralizes
* shared constants and helper behavior used by concrete implementations.
*
* Main responsibilities:
* - define the abstract skeletonizer algorithm contract
* - provide shared foreground, background, and skeleton constants
* - convert skeletonizer backend types to and from string representations
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <string>
#include <optional>

#include "opencv2/core.hpp"

#include "SkeletonizationCore/extensions/string.hpp"

namespace skeletonizer
{
	/**
	 * @class skeletonizer
	 * @brief Represents an abstract interface for skeletonization algorithms.
	 *
	 * This template defines the contract for applying thinning operations on
	 * binary images and exposes compile-time pixel semantics used by algorithms.
	 */
	template <int ForegroundValue = 1, int SkeletonValue = 1, int BackgroundValue = 0>
	class skeletonizer
	{
	public:
		/**
		 * @brief Destroys the skeletonizer instance.
		 */
		virtual ~skeletonizer() = default;

		/**
		 * @brief Applies the skeletonization algorithm to a binary image.
		 *
		 * @param binary_image Binary image modified in place.
		 */
		virtual void apply(cv::Mat& binary_image) const = 0;

		/**
		 * @brief Returns the algorithm display name.
		 *
		 * @return Human-readable algorithm name.
		 */
		virtual std::string name() const = 0;

		/// Foreground pixel value used by the algorithm.
		static constexpr int foreground = ForegroundValue;
		/// Background pixel value used by the algorithm.
		static constexpr int background = BackgroundValue;
		/// Skeleton pixel value used by the algorithm.
		static constexpr int skeleton = SkeletonValue;

	protected:
		/**
		 * @brief Checks whether any pixels changed in the difference image.
		 *
		 * @param difference Difference image between iterations.
		 * @return True when at least one pixel changes.
		 */
		virtual bool has_changed(cv::InputArray& difference) const
		{
			return cv::countNonZero(difference) > 0;
		}

		/**
		 * @brief Clears all border pixels of a binary image.
		 *
		 * @param binary_image Binary image whose border is set to zero.
		 */
		static void clear_border(const cv::Mat& binary_image)
		{
			binary_image.row(0).setTo(0);
			binary_image.row(binary_image.rows - 1).setTo(0);
			binary_image.col(0).setTo(0);
			binary_image.col(binary_image.cols - 1).setTo(0);
		}
	};

	/**
	 * @brief Enumerates available skeletonizer backend types.
	 */
	enum class skeletonizer_type
	{
		cpu,
		thread
#if SKELETONIZATION_WITH_GPU
		gpu
#endif
	};

	/**
	 * @brief Converts backend type to lowercase string view.
	 *
	 * @param type Backend type value.
	 * @return Lowercase backend type name.
	 */
	[[nodiscard]] constexpr std::string_view to_string_view(const skeletonizer_type type) noexcept
	{
		using enum skeletonizer_type;

		switch (type)
		{
		case cpu: return "cpu";
		case thread: return "thread";
#if SKELETONIZATION_WITH_GPU
		case gpu: return "gpu";
#endif
		default: return "unknown";
		}
	}

	/**
	 * @brief Converts backend type to string.
	 *
	 * @param type Backend type value.
	 * @param to_upper Converts result to uppercase when true.
	 * @return Backend type name string.
	 */
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

	/**
	 * @brief Parses backend type from string.
	 *
	 * @param value Backend type name.
	 * @return Parsed backend type when value is valid.
	 */
	[[nodiscard]] inline std::optional<skeletonizer_type> from_string(const std::string_view value)
	{
#if SKELETONIZATION_WITH_GPU
		if (equals_ascii(value, "gpu"))
		{
			return skeletonizer_type::gpu;
		}
#endif
		if (equals_ascii(value, "thread"))
		{
			return skeletonizer_type::thread;
		}

		if (equals_ascii(value, "cpu"))
		{
			return skeletonizer_type::cpu;
		}

		return std::nullopt;
	}
}
