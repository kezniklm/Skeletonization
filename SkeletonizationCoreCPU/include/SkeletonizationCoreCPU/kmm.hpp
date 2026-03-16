/**
*
* @file kmm.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares the CPU kmm thinning algorithm.
*
* This header defines the CPU kmm implementation interface, local region
* tracking utilities, and lookup tables used by staged deletion passes.
*
* Main responsibilities:
* - declare the CPU kmm algorithm class
* - define region-of-interest tracking helpers
* - provide lookup tables and stage operations for thinning
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <algorithm>
#include <array>
#include <cstdint>

#include <opencv2/core.hpp>

#include "SkeletonizationCore/skeletonizer/algorithms.hpp"
#include "SkeletonizationCore/skeletonizer/backend.hpp"

namespace skeletonizer::cpu::algorithms
{
	/**
	 * @class kmm
	 * @brief Implements kmm thinning on CPU.
	 *
	 * This class performs staged marking and deletion operations in iterative
	 * passes while restricting work to active regions.
	 */
	class kmm final : public backend_cpu, public ::skeletonizer::algorithms::kmm
	{
	public:
		/**
		 * @brief Applies kmm thinning to a binary image.
		 *
		 * @param binary_image Binary image modified in place.
		 */
		void apply(cv::Mat& binary_image) const override;

	private:
		/// Label value for edge candidates.
		static constexpr auto edge = 2;
		/// Label value for corner candidates.
		static constexpr auto corner = 3;
		/// Label value for temporary marks.
		static constexpr auto mark = 4;

		/// Halo size for touched region expansion.
		static constexpr auto halo_touch = 1;
		/// Halo size for collapse operations.
		static constexpr auto halo_collapse = 1;
		/// Halo size for next region-of-interest.
		static constexpr auto halo_next_roi = 2;

		/**
		 * @brief Creates working matrix for kmm stages.
		 *
		 * @param binary_image Source binary image.
		 * @return Workspace matrix.
		 */
		static cv::Mat create_workspace_matrix(const cv::Mat& binary_image);

		/**
		 * @class region_of_interest
		 * @brief Represents an active matrix subregion.
		 *
		 * This structure bounds stage processing to modified neighborhoods.
		 */
		struct region_of_interest
		{
			/// Inclusive start row.
			int y0{0}, y1{0};
			/// Inclusive start column.
			int x0{0}, x1{0};

			/**
			 * @brief Checks whether region is empty.
			 *
			 * @return True when region has no area.
			 */
			bool empty() const { return y0 >= y1 || x0 >= x1; }

			/**
			 * @brief Returns region expanded by halo and clamped to bounds.
			 *
			 * @param halo Expansion halo size.
			 * @param h Matrix height.
			 * @param w Matrix width.
			 * @return Expanded region.
			 */
			region_of_interest expanded(const int halo, const int h, const int w) const
			{
				if (empty())
				{
					return *this;
				}

				region_of_interest roi;

				roi.y0 = std::max(1, y0 - halo);
				roi.y1 = std::min(h - 1, y1 + halo);
				roi.x0 = std::max(1, x0 - halo);
				roi.x1 = std::min(w - 1, x1 + halo);

				return roi;
			}

			/**
			 * @brief Expands region to include a row span.
			 *
			 * @param y Row index.
			 * @param x_begin Start column.
			 * @param x_end End column.
			 * @param clamp_to Bounding region.
			 */
			void widen_to_include_row_span(const int y, const int x_begin, const int x_end,
			                               const region_of_interest& clamp_to)
			{
				if (x_begin > x_end)
				{
					return;
				}

				if (empty())
				{
					y0 = y;
					y1 = y + 1;
					x0 = std::clamp(x_begin, clamp_to.x0, clamp_to.x1);
					x1 = std::clamp(x_end + 1, clamp_to.x0, clamp_to.x1);

					return;
				}

				y0 = std::min(y0, y);
				y1 = std::max(y1, y + 1);
				x0 = std::min(x0, std::max(clamp_to.x0, x_begin));
				x1 = std::max(x1, std::min(clamp_to.x1, x_end + 1));
			}
		};

		/**
		 * @brief Collapses workspace labels to binary values inside region.
		 *
		 * @param matrix Workspace matrix.
		 * @param roi Active region.
		 */
		static void collapse_to_binary(const cv::Mat& matrix, const region_of_interest& roi);

		/**
		 * @brief Computes neighborhood bit mask.
		 *
		 * @param previous Previous-row pointer.
		 * @param current Current-row pointer.
		 * @param next Next-row pointer.
		 * @param x Column index.
		 * @return Encoded neighborhood mask.
		 */
		static uint8_t neighbour_mask(const uint8_t* previous,
		                              const uint8_t* current,
		                              const uint8_t* next,
		                              int x);

		/**
		 * @brief Labels edge and corner candidates.
		 *
		 * @param matrix Workspace matrix.
		 * @param roi Active region.
		 */
		static void stage2_label(cv::Mat& matrix, const region_of_interest& roi);
		/**
		 * @brief Marks stick pixels for preservation.
		 *
		 * @param matrix Workspace matrix.
		 * @param roi Active region.
		 */
		static void stage3_mark_stick(cv::Mat& matrix, const region_of_interest& roi);
		/**
		 * @brief Deletes marked pixels and returns touched region.
		 *
		 * @param matrix Workspace matrix.
		 * @param roi Active region.
		 * @param touched_out Output touched region.
		 * @return True when at least one pixel is deleted.
		 */
		static bool stage4_delete_marked(cv::Mat& matrix, const region_of_interest& roi,
		                                 region_of_interest& touched_out);
		/**
		 * @brief Deletes target-labeled pixels using lookup rules.
		 *
		 * @param m Workspace matrix.
		 * @param target Target label.
		 * @param roi Active region.
		 * @param touched_out Output touched region.
		 * @return True when at least one pixel is deleted.
		 */
		static bool stage5_delete_array(cv::Mat& m, uint8_t target,
		                                const region_of_interest& roi,
		                                region_of_interest& touched_out);

		/// Neighborhood values treated as sticks.
		static constexpr uint8_t stick_values[] =
		{
			3, 6, 12, 24, 48, 96, 192, 129,
			7, 14, 28, 56, 112, 224, 193, 131,
			15, 30, 60, 120, 240, 225, 195, 135
		};

		/**
		 * @brief Builds lookup table for stick values.
		 *
		 * @return Stick lookup table.
		 */
		static constexpr std::array<uint8_t, 256> make_stick_lut()
		{
			std::array<uint8_t, 256> array{};

			for (const auto v : stick_values)
			{
				array[v] = 1;
			}

			return array;
		}

		/// Lookup table for stick values.
		static inline const std::array<uint8_t, 256> stick_lut = make_stick_lut();

		/// Neighborhood values eligible for deletion.
		static constexpr uint8_t deletion_array[] =
		{
			3, 5, 7, 12, 13, 14, 15, 20, 21, 22, 23, 28, 29, 30, 31, 48,
			52, 53, 54, 55, 56, 60, 61, 62, 63, 65, 67, 69, 71, 77, 79, 80,
			81, 83, 84, 85, 86, 87, 88, 89, 91, 92, 93, 94, 95, 97, 99, 101,
			103, 109, 111, 112, 113, 115, 116, 117, 118, 119, 120, 121, 123, 124, 125, 126,
			127, 131, 133, 135, 141, 143, 149, 151, 157, 159, 181, 183, 189, 191, 192, 193,
			195, 197, 199, 205, 207, 208, 209, 211, 212, 213, 214, 215, 216, 217, 219, 220,
			221, 222, 223, 224, 225, 227, 229, 231, 237, 239, 240, 241, 243, 244, 245, 246,
			247, 248, 249, 251, 252, 253, 254, 255
		};

		/**
		 * @brief Builds lookup table for deletion values.
		 *
		 * @return Deletion lookup table.
		 */
		static constexpr std::array<uint8_t, 256> make_delete_lut()
		{
			std::array<uint8_t, 256> array{};

			for (const auto v : deletion_array)
			{
				array[v] = 1;
			}

			return array;
		}

		/// Lookup table for deletion values.
		static inline const std::array<uint8_t, 256> delete_lut = make_delete_lut();
	};
}
