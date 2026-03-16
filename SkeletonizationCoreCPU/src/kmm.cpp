/**
*
* @file kmm.cpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Implements the CPU kmm thinning algorithm.
*
* This file performs staged kmm labeling and deletion inside an active
* region-of-interest to accelerate iterative convergence.
*
* Main responsibilities:
* - run staged kmm thinning loop with active region updates
* - label, mark, and delete candidate pixels by lookup masks
* - collapse workspace labels back to binary representation
*
* @version 1.0
* @date 2026-03-16
*/

#include "SkeletonizationCoreCPU/kmm.hpp"

namespace skeletonizer::cpu::algorithms
{
	/**
	 * @brief Applies kmm thinning until no more deletions occur.
	 *
	 * @param binary_image Binary image modified in place.
	 */
	void kmm::apply(cv::Mat& binary_image) const
	{
		auto workspace = create_workspace_matrix(binary_image);

		region_of_interest roi{.y0 = 1, .y1 = workspace.rows - 1, .x0 = 1, .x1 = workspace.cols - 1};

		auto any_deleted = true;

		auto guard = std::max(200, (workspace.rows - 2) * (workspace.cols - 2));

		while (any_deleted && guard-- > 0)
		{
			any_deleted = false;

			stage2_label(workspace, roi);

			stage3_mark_stick(workspace, roi);

			region_of_interest touched;

			any_deleted |= stage4_delete_marked(workspace, roi, touched);
			any_deleted |= stage5_delete_array(workspace, edge, roi, touched);
			any_deleted |= stage5_delete_array(workspace, corner, roi, touched);

			if (touched.empty())
			{
				continue;
			}

			collapse_to_binary(workspace, touched.expanded(1, workspace.rows, workspace.cols));

			roi = touched.expanded(halo_next_roi, workspace.rows, workspace.cols);
		}

		workspace(cv::Rect(1, 1, binary_image.cols, binary_image.rows)).copyTo(binary_image);
	}

	/**
	 * @brief Creates a bordered workspace matrix for staged operations.
	 *
	 * @param binary_image Source binary image.
	 * @return Workspace with one-pixel border.
	 */
	cv::Mat kmm::create_workspace_matrix(const cv::Mat& binary_image)
	{
		cv::Mat workspace;

		cv::copyMakeBorder(binary_image, workspace, 1, 1, 1, 1, cv::BORDER_CONSTANT, cv::Scalar(0));

		return workspace;
	}

	/**
	 * @brief Collapses workspace labels to binary values in a region.
	 *
	 * @param matrix Workspace matrix.
	 * @param roi Active region.
	 */
	void kmm::collapse_to_binary(const cv::Mat& matrix, const region_of_interest& roi)
	{
		if (roi.y0 >= roi.y1 || roi.x0 >= roi.x1)
		{
			return;
		}

		const cv::Rect rc(roi.x0, roi.y0,
		                  roi.x1 - roi.x0,
		                  roi.y1 - roi.y0);

		cv::Mat sub = matrix(rc);

		cv::threshold(sub, sub, background, foreground, cv::THRESH_BINARY);
	}

	/**
	 * @brief Computes neighborhood bit mask around pixel x.
	 *
	 * @param previous Pointer to previous row.
	 * @param current Pointer to current row.
	 * @param next Pointer to next row.
	 * @param x Column index.
	 * @return Encoded neighborhood mask.
	 */
	uint8_t kmm::neighbour_mask(const uint8_t* previous,
	                            const uint8_t* current,
	                            const uint8_t* next,
	                            int x)
	{
		return (previous[x - 1] != 0) << 7 |
			(previous[x + 0] != 0) << 0 |
			(previous[x + 1] != 0) << 1 |
			(current[x - 1] != 0) << 6 |
			(current[x + 1] != 0) << 2 |
			(next[x - 1] != 0) << 5 |
			(next[x + 0] != 0) << 4 |
			(next[x + 1] != 0) << 3;
	}

	/**
	 * @brief Labels edge and corner candidates in active region.
	 *
	 * @param matrix Workspace matrix.
	 * @param roi Active region.
	 */
	void kmm::stage2_label(cv::Mat& matrix, const region_of_interest& roi)
	{
		for (auto y = roi.y0; y < roi.y1; ++y)
		{
			const auto previous = matrix.ptr<uint8_t>(y - 1);
			const auto current = matrix.ptr<uint8_t>(y + 0);
			const auto next = matrix.ptr<uint8_t>(y + 1);

			for (auto x = roi.x0; x < roi.x1; ++x)
			{
				if (current[x] != foreground)
				{
					continue;
				}

				const bool all4 = previous[x] && current[x - 1] && current[x + 1] && next[x];

				if (!all4)
				{
					current[x] = edge;
					continue;
				}

				if (!(previous[x - 1] && previous[x + 1] && next[x - 1] && next[x + 1]))
				{
					current[x] = corner;
				}
			}
		}
	}

	/**
	 * @brief Marks stick pixels that must be preserved.
	 *
	 * @param matrix Workspace matrix.
	 * @param roi Active region.
	 */
	void kmm::stage3_mark_stick(cv::Mat& matrix, const region_of_interest& roi)
	{
		for (auto y = roi.y0; y < roi.y1; ++y)
		{
			const auto previous = matrix.ptr<uint8_t>(y - 1);
			const auto current = matrix.ptr<uint8_t>(y + 0);
			const auto next = matrix.ptr<uint8_t>(y + 1);

			for (auto x = roi.x0; x < roi.x1; ++x)
			{
				const auto v = current[x];

				if (v != edge && v != corner)
				{
					continue;
				}

				const auto s = neighbour_mask(previous, current, next, x);

				if (stick_lut[s])
				{
					current[x] = mark;
				}
			}
		}
	}

	/**
	 * @brief Deletes marked pixels and reports touched region.
	 *
	 * @param matrix Workspace matrix.
	 * @param roi Active region.
	 * @param touched_out Output touched region.
	 * @return True when at least one pixel is deleted.
	 */
	bool kmm::stage4_delete_marked(cv::Mat& matrix, const region_of_interest& roi,
	                               region_of_interest& touched_out)
	{
		bool any = false;

		for (auto y = roi.y0; y < roi.y1; ++y)
		{
			const auto current = matrix.ptr<uint8_t>(y);

			auto x_first = roi.x1;
			auto x_last = roi.x0 - 1;

			for (auto x = roi.x0; x < roi.x1; ++x)
			{
				if (current[x] != mark)
				{
					continue;
				}

				current[x] = background;
				any = true;
				x_first = std::min(x_first, x);
				x_last = std::max(x_last, x);
			}

			if (x_first <= x_last)
			{
				touched_out.widen_to_include_row_span(
					y, x_first - halo_touch, x_last + halo_touch, roi);
			}
		}

		return any;
	}

	/**
	 * @brief Deletes pixels matching target class and delete lookup table.
	 *
	 * @param m Workspace matrix.
	 * @param target Target label.
	 * @param roi Active region.
	 * @param touched_out Output touched region.
	 * @return True when at least one pixel is deleted.
	 */
	bool kmm::stage5_delete_array(cv::Mat& m, uint8_t target,
	                              const region_of_interest& roi,
	                              region_of_interest& touched_out)
	{
		bool any = false;

		for (auto y = roi.y0; y < roi.y1; ++y)
		{
			const auto previous = m.ptr<uint8_t>(y - 1);
			const auto current = m.ptr<uint8_t>(y + 0);
			const auto next = m.ptr<uint8_t>(y + 1);

			auto x_first = roi.x1;
			auto x_last = roi.x0 - 1;

			for (auto x = roi.x0; x < roi.x1; ++x)
			{
				if (current[x] != target)
				{
					continue;
				}

				const auto s = neighbour_mask(previous, current, next, x);

				if (!delete_lut[s])
				{
					current[x] = foreground;
				}
				else
				{
					current[x] = background;
					any = true;
					x_first = std::min(x_first, x);
					x_last = std::max(x_last, x);
				}
			}

			if (x_first <= x_last)
			{
				touched_out.widen_to_include_row_span(
					y, x_first - halo_touch, x_last + halo_touch, roi);
			}
		}

		return any;
	}
}
