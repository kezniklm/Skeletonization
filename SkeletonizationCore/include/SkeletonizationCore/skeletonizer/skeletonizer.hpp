#pragma once

#include <string>
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

	inline std::string to_string(const skeletonizer_type skeletonizer_type)
	{
		switch (skeletonizer_type)
		{
		case skeletonizer_type::cpu:
			return "CPU";
		case skeletonizer_type::thread:
			return "THREAD";
		case skeletonizer_type::gpu:
			return "GPU";
		default:
			return "Unknown";
		}
	}
}
