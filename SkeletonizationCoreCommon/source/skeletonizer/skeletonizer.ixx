module;

#include <opencv2/core.hpp>

export module skeletonizer:core;

namespace skeletonizer
{
	export class skeletonizer
	{
	public:
		virtual ~skeletonizer() = default;

		virtual cv::Mat apply(const cv::Mat& binary_image) const = 0;

		virtual std::string name() const = 0;
	};

	export enum class skeletonizer_type
	{
		cpu,
		gpu
	};

	export inline std::string to_string(const skeletonizer_type skeletonizer_type)
	{
		switch (skeletonizer_type)
		{
		case skeletonizer_type::cpu:
			return "CPU";
		case skeletonizer_type::gpu:
			return "GPU";
		default:
			return "Unknown";
		}
	}
}
