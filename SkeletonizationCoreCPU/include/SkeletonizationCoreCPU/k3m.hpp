/**
*
* @file k3m.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares the CPU k3m thinning algorithm and lookup tables.
*
* This header defines the CPU k3m implementation interface, phase lookup
* tables, and helper routines used for staged pixel deletion.
*
* Main responsibilities:
* - declare the CPU k3m algorithm class
* - define compile-time phase lookup tables
* - expose helper routines for candidate processing
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <array>
#include <cstdint>
#include <vector>

#include <opencv2/core.hpp>

#include "SkeletonizationCore/skeletonizer/backend.hpp"
#include "SkeletonizationCore/skeletonizer/algorithms.hpp"

namespace skeletonizer::cpu::algorithms
{
	/**
	 * @brief Builds a 256-entry lookup table with selected active indices.
	 *
	 * @return Lookup table where selected entries are set to one.
	 */
	template <int... Values>
	consteval std::array<std::uint8_t, 256> build_lut()
	{
		std::array<std::uint8_t, 256> lut{};
		((lut[static_cast<std::size_t>(Values)] = 1), ...);
		return lut;
	}

	/**
	 * @class k3m
	 * @brief Implements k3m thinning on CPU.
	 *
	 * This class runs phase-based deletion passes using precomputed lookup
	 * tables and candidate sets.
	 */
	class k3m final : public backend_cpu, public ::skeletonizer::algorithms::k3m
	{
	public:
		/// Lookup table for phase A0.
		static constexpr auto A0 = build_lut<
			3, 6, 7, 12, 14, 15, 24, 28, 30, 31, 48, 56, 60, 62, 63, 96,
			112, 120, 124, 126, 127, 129, 131, 135, 143, 159, 191, 192,
			193, 195, 199, 207, 223, 224, 225, 227, 231, 239, 240, 241,
			243, 247, 248, 249, 251, 252, 253, 254>();

		/// Lookup table for phase A1.
		static constexpr auto A1 = build_lut<
			7, 14, 28, 56, 112, 131, 193, 224>();

		/// Lookup table for phase A2.
		static constexpr auto A2 = build_lut<
			7, 14, 15, 28, 30, 56, 60, 112, 120, 131, 135, 193, 195, 224,
			225, 240>();

		/// Lookup table for phase A3.
		static constexpr auto A3 = build_lut<
			7, 14, 15, 28, 30, 31, 56, 60, 62, 112, 120, 124, 131, 135,
			143, 193, 195, 199, 224, 225, 227, 240, 241, 248>();

		/// Lookup table for phase A4.
		static constexpr auto A4 = build_lut<
			7, 14, 15, 28, 30, 31, 56, 60, 62, 63, 112, 120, 124, 126,
			131, 135, 143, 159, 193, 195, 199, 207, 224, 225, 227, 231,
			240, 241, 243, 248, 249, 252>();

		/// Lookup table for phase A5.
		static constexpr auto A5 = build_lut<
			7, 14, 15, 28, 30, 31, 56, 60, 62, 63, 112, 120, 124, 126,
			131, 135, 143, 159, 191, 193, 195, 199, 207, 224, 225, 227,
			231, 239, 240, 241, 243, 248, 249, 251, 252, 254>();

		/// Lookup table for one-pixel-width postprocessing.
		static constexpr auto A1pix = build_lut<
			3, 6, 7, 12, 14, 15, 24, 28, 30, 31, 48, 56, 60, 62, 63, 96,
			112, 120, 124, 126, 127, 129, 131, 135, 143, 159, 191, 192,
			193, 195, 199, 207, 223, 224, 225, 227, 231, 239, 240, 241,
			243, 247, 248, 249, 251, 252, 253, 254>();

		/**
		 * @brief Applies k3m thinning to a binary image.
		 *
		 * @param binary_image Binary image modified in place.
		 */
		void apply(cv::Mat& binary_image) const override;

	private:
		/**
		 * @class candidate
		 * @brief Stores candidate pixel coordinates.
		 *
		 * This structure identifies foreground pixels selected for deletion checks.
		 */
		struct candidate
		{
			/// Candidate row index.
			int row;
			/// Candidate column index.
			int column;
		};

		/**
		 * @brief Calculates neighborhood weight for a pixel.
		 *
		 * @param upper_row Pointer to upper row data.
		 * @param current_row Pointer to current row data.
		 * @param lower_row Pointer to lower row data.
		 * @param column Column index.
		 * @return Encoded neighborhood weight.
		 */
		static std::uint8_t calculate_neighbour_weight(const std::uint8_t* upper_row,
		                                               const std::uint8_t* current_row,
		                                               const std::uint8_t* lower_row,
		                                               int column) noexcept;

		/**
		 * @brief Collects initial deletion candidates.
		 *
		 * @param binary_image Source binary image.
		 * @param candidates Output candidate collection.
		 */
		static void phase_0(const cv::Mat& binary_image,
		                    std::vector<candidate>& candidates);

		/**
		 * @brief Deletes candidates matching phase lookup table.
		 *
		 * @param binary_image Binary image modified in place.
		 * @param candidates Candidate pixels to evaluate.
		 * @param Ai Phase lookup table.
		 * @return True when at least one pixel is deleted.
		 */
		static bool delete_phase(cv::Mat& binary_image,
		                         const std::vector<candidate>& candidates,
		                         const std::array<std::uint8_t, 256>& Ai);

		/**
		 * @brief Applies one-pixel-width final thinning step.
		 *
		 * @param binary_image Binary image modified in place.
		 */
		static void thin_to_one_pixel_width(cv::Mat& binary_image);
	};
}
