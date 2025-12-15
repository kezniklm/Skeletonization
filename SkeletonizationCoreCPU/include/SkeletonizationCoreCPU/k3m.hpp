#pragma once

#include <array>
#include <cstdint>
#include <vector>

#include <opencv2/core.hpp>

#include "SkeletonizationCore/skeletonizer/backend.hpp"
#include "SkeletonizationCore/skeletonizer/algorithms.hpp"

namespace skeletonizer::cpu::algorithms
{
	template <int... Values>
	consteval std::array<std::uint8_t, 256> build_lut()
	{
		std::array<std::uint8_t, 256> lut{};
		((lut[static_cast<std::size_t>(Values)] = 1), ...);
		return lut;
	}

	class k3m final : public backend_cpu, public ::skeletonizer::algorithms::k3m
	{
	public:
		static constexpr auto A0 = build_lut<
			3, 6, 7, 12, 14, 15, 24, 28, 30, 31, 48, 56, 60, 62, 63, 96,
			112, 120, 124, 126, 127, 129, 131, 135, 143, 159, 191, 192,
			193, 195, 199, 207, 223, 224, 225, 227, 231, 239, 240, 241,
			243, 247, 248, 249, 251, 252, 253, 254>();

		static constexpr auto A1 = build_lut<
			7, 14, 28, 56, 112, 131, 193, 224>();

		static constexpr auto A2 = build_lut<
			7, 14, 15, 28, 30, 56, 60, 112, 120, 131, 135, 193, 195, 224,
			225, 240>();

		static constexpr auto A3 = build_lut<
			7, 14, 15, 28, 30, 31, 56, 60, 62, 112, 120, 124, 131, 135,
			143, 193, 195, 199, 224, 225, 227, 240, 241, 248>();

		static constexpr auto A4 = build_lut<
			7, 14, 15, 28, 30, 31, 56, 60, 62, 63, 112, 120, 124, 126,
			131, 135, 143, 159, 193, 195, 199, 207, 224, 225, 227, 231,
			240, 241, 243, 248, 249, 252>();

		static constexpr auto A5 = build_lut<
			7, 14, 15, 28, 30, 31, 56, 60, 62, 63, 112, 120, 124, 126,
			131, 135, 143, 159, 191, 193, 195, 199, 207, 224, 225, 227,
			231, 239, 240, 241, 243, 248, 249, 251, 252, 254>();

		static constexpr auto A1pix = build_lut<
			3, 6, 7, 12, 14, 15, 24, 28, 30, 31, 48, 56, 60, 62, 63, 96,
			112, 120, 124, 126, 127, 129, 131, 135, 143, 159, 191, 192,
			193, 195, 199, 207, 223, 224, 225, 227, 231, 239, 240, 241,
			243, 247, 248, 249, 251, 252, 253, 254>();

		void apply(cv::Mat& binary_image) const override;

	private:
		struct candidate
		{
			int row;
			int column;
		};

		static std::uint8_t calculate_neighbour_weight(const std::uint8_t* upper_row,
		                                               const std::uint8_t* current_row,
		                                               const std::uint8_t* lower_row,
		                                               int column) noexcept;

		static void phase_0(const cv::Mat& binary_image,
		                    std::vector<candidate>& candidates);

		static bool delete_phase(cv::Mat& binary_image,
		                         const std::vector<candidate>& candidates,
		                         const std::array<std::uint8_t, 256>& Ai);

		static void thin_to_one_pixel_width(cv::Mat& binary_image);
	};
}
