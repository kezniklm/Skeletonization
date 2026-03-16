/**
*
* @file k3m.cpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Implements the CPU k3m thinning algorithm.
*
* This file executes phase-based deletion passes and a final one-pixel-width
* cleanup step using precomputed lookup tables.
*
* Main responsibilities:
* - run k3m phase-based thinning loop
* - compute neighborhood weights for candidate pixels
* - apply final one-pixel-width thinning step
*
* @version 1.0
* @date 2026-03-16
*/

#include "SkeletonizationCoreCPU/k3m.hpp"

namespace skeletonizer::cpu::algorithms
{
	/**
	 * @brief Applies k3m thinning until no more deletions occur.
	 *
	 * @param binary_image Binary image modified in place.
	 */
	void k3m::apply(cv::Mat& binary_image) const
	{
		clear_border(binary_image);

		std::vector<candidate> candidates;
		candidates.reserve(binary_image.rows * binary_image.cols / 2);

		bool any_deleted;
		do
		{
			phase_0(binary_image, candidates);

			any_deleted = delete_phase(binary_image, candidates, A1);
			any_deleted |= delete_phase(binary_image, candidates, A2);
			any_deleted |= delete_phase(binary_image, candidates, A3);
			any_deleted |= delete_phase(binary_image, candidates, A4);
			any_deleted |= delete_phase(binary_image, candidates, A5);
			// Phase 6 (unmark) is implicit because we rebuild candidates each loop.
		}
		while (any_deleted);

		thin_to_one_pixel_width(binary_image);
	}

	/**
	 * @brief Calculates neighborhood weight mask for k3m lookup.
	 *
	 * @param upper_row Pointer to upper row data.
	 * @param current_row Pointer to current row data.
	 * @param lower_row Pointer to lower row data.
	 * @param column Column index.
	 * @return Neighborhood bit mask.
	 */
	std::uint8_t k3m::calculate_neighbour_weight(const std::uint8_t* upper_row,
	                                             const std::uint8_t* current_row,
	                                             const std::uint8_t* lower_row,
	                                             const int column) noexcept
	{
		std::uint8_t mask = 0;
		mask |= upper_row[column] != 0 ? 1 : 0;
		mask |= upper_row[column + 1] != 0 ? 2 : 0;
		mask |= current_row[column + 1] != 0 ? 4 : 0;
		mask |= lower_row[column + 1] != 0 ? 8 : 0;
		mask |= lower_row[column] != 0 ? 16 : 0;
		mask |= lower_row[column - 1] != 0 ? 32 : 0;
		mask |= current_row[column - 1] != 0 ? 64 : 0;
		mask |= upper_row[column - 1] != 0 ? 128 : 0;
		return mask;
	}

	/**
	 * @brief Collects phase-0 candidate pixels.
	 *
	 * @param binary_image Source binary image.
	 * @param candidates Output candidate collection.
	 */
	void k3m::phase_0(const cv::Mat& binary_image,
	                  std::vector<candidate>& candidates)
	{
		candidates.clear();

		for (int row = 1; row < binary_image.rows - 1; ++row)
		{
			const auto* upper_row = binary_image.ptr<std::uint8_t>(row - 1);
			const auto* current_row = binary_image.ptr<std::uint8_t>(row);
			const auto* lower_row = binary_image.ptr<std::uint8_t>(row + 1);

			for (int col = 1; col < binary_image.cols - 1; ++col)
			{
				if (current_row[col] == background)
				{
					continue;
				}

				const auto w =
					calculate_neighbour_weight(upper_row, current_row, lower_row, col);

				if (A0[w])
				{
					candidates.push_back({row, col});
				}
			}
		}
	}

	/**
	 * @brief Deletes candidate pixels matching phase lookup table.
	 *
	 * @param binary_image Binary image modified in place.
	 * @param candidates Candidate pixels to evaluate.
	 * @param Ai Phase lookup table.
	 * @return True when at least one pixel is deleted.
	 */
	bool k3m::delete_phase(cv::Mat& binary_image,
	                       const std::vector<candidate>& candidates,
	                       const std::array<std::uint8_t, 256>& Ai)
	{
		bool deleted = false;

		int current_row_number = std::numeric_limits<int>::min();

		const std::uint8_t* upper_row = nullptr;
		std::uint8_t* current_row = nullptr;
		const std::uint8_t* lower_row = nullptr;

		for (const auto& [row, column] : candidates)
		{
			if (row != current_row_number)
			{
				current_row_number = row;
				upper_row = binary_image.ptr<std::uint8_t>(current_row_number - 1);
				current_row = binary_image.ptr<std::uint8_t>(current_row_number);
				lower_row = binary_image.ptr<std::uint8_t>(current_row_number + 1);
			}

			if (current_row[column] == background)
			{
				continue;
			}

			const auto w =
				calculate_neighbour_weight(upper_row, current_row, lower_row, column);

			if (!Ai[w])
			{
				continue;
			}

			current_row[column] = background;
			deleted = true;
		}

		return deleted;
	}

	/**
	 * @brief Applies final one-pixel-width thinning cleanup.
	 *
	 * @param binary_image Binary image modified in place.
	 */
	void k3m::thin_to_one_pixel_width(cv::Mat& binary_image)
	{
		bool changed;

		do
		{
			changed = false;

			for (int row = 1; row < binary_image.rows - 1; ++row)
			{
				const auto* upper_row = binary_image.ptr<std::uint8_t>(row - 1);
				auto* current_row = binary_image.ptr<std::uint8_t>(row);
				const auto* lower_row = binary_image.ptr<std::uint8_t>(row + 1);

				for (int column = 1; column < binary_image.cols - 1; ++column)
				{
					if (current_row[column] == background)
					{
						continue;
					}

					const auto w =
						calculate_neighbour_weight(upper_row, current_row, lower_row, column);

					if (A1pix[w])
					{
						current_row[column] = background;
						changed = true;
					}
				}
			}
		}
		while (changed);
	}
}
