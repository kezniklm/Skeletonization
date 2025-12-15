#pragma once

#include <array>
#include <cstdint>

#include <opencv2/core.hpp>

#include "SkeletonizationCore/skeletonizer/backend.hpp"
#include "SkeletonizationCore/skeletonizer/algorithms.hpp"

namespace skeletonizer::cpu::algorithms
{
	class tarabek final : public backend_cpu, public ::skeletonizer::algorithms::tarabek
	{
	public:
		void apply(cv::Mat& binary_image) const override;

	private:
		static void first_iteration(cv::Mat& binary_image, cv::Mat& marker);
		static void second_iteration(cv::Mat& binary_image, cv::Mat& marker);

		template <std::size_t N>
		static void process_template_set(
			cv::Mat& binary_image,
			const std::array<mask_bits, N>& templates_bits,
			std::uint8_t background_value);

		static void postprocessing(cv::Mat& binary_image);
	};
}
