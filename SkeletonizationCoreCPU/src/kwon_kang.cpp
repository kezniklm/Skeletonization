/**
*
* @file kwon_kang.cpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Implements the CPU kwon-gi-kang thinning algorithm.
*
* This file applies two iterative deletion passes with additional corner
* cleanup to preserve shape continuity.
*
* Main responsibilities:
* - run kwon-gi-kang iterative thinning loop
* - execute first and second deletion passes
* - remove oblique corner artifacts after each cycle
*
* @version 1.0
* @date 2026-03-16
*/

#include <SkeletonizationCoreCPU/kwon_kang.hpp>

namespace skeletonizer::cpu::algorithms
{
	/**
	 * @brief Applies kwon-gi-kang thinning until convergence.
	 *
	 * @param binary_image Binary image modified in place.
	 */
	void kwon_kang::apply(cv::Mat& binary_image) const
	{
		cv::Mat previous(binary_image.size(), CV_8UC1, cv::Scalar(0));
		cv::Mat marker(binary_image.size(), CV_8UC1);
		cv::Mat difference;

		do
		{
			first_iteration(binary_image, marker);
			second_iteration(binary_image, marker);

			cv::absdiff(binary_image, previous, difference);
			binary_image.copyTo(previous);
		}
		while (has_changed(difference));

		cleanup_oblique_corners(binary_image, marker);

		clear_border(binary_image);
	}

	/**
	 * @brief Executes the first kwon-gi-kang deletion pass.
	 *
	 * @param binary_image Binary image modified in place.
	 * @param marker Temporary marker matrix.
	 */
	void kwon_kang::first_iteration(cv::Mat& binary_image, cv::Mat& marker)
	{
		marker.setTo(0);

		for (int row = 1; row < binary_image.rows - 1; ++row)
		{
			const uchar* previous = binary_image.ptr<uchar>(row - 1);
			const uchar* current = binary_image.ptr<uchar>(row);
			const uchar* next = binary_image.ptr<uchar>(row + 1);
			uchar* marker_pointer = marker.ptr<uchar>(row);

			for (int column = 1; column < binary_image.cols - 1; ++column)
			{
				const auto pi = current[column];

				if (pi == background)
				{
					continue;
				}

				const auto p1 = previous[column - 1];
				const auto p2 = previous[column];
				const auto p3 = previous[column + 1];
				const auto p4 = current[column + 1];
				const auto p5 = next[column + 1];
				const auto p6 = next[column];
				const auto p7 = next[column - 1];
				const auto p8 = current[column - 1];

				const auto a =
					(p1 == 0 && p2 == 1) +
					(p2 == 0 && p3 == 1) +
					(p3 == 0 && p4 == 1) +
					(p4 == 0 && p5 == 1) +
					(p5 == 0 && p6 == 1) +
					(p6 == 0 && p7 == 1) +
					(p7 == 0 && p8 == 1) +
					(p8 == 0 && p1 == 1);

				const auto b =
					p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8;

				const auto step_condition_c = p2 * p4 * p6;
				const auto step_condition_d = p4 * p6 * p8;

				if (a == 1 && b >= 2 && b <= 6 &&
					step_condition_c == 0 && step_condition_d == 0)
				{
					marker_pointer[column] = skeleton;
				}
			}
		}

		binary_image &= ~marker;
	}

	/**
	 * @brief Executes the second kwon-gi-kang deletion pass.
	 *
	 * @param binary_image Binary image modified in place.
	 * @param marker Temporary marker matrix.
	 */
	void kwon_kang::second_iteration(cv::Mat& binary_image, cv::Mat& marker)
	{
		marker.setTo(0);

		for (int row = 1; row < binary_image.rows - 1; ++row)
		{
			const uchar* previous = binary_image.ptr<uchar>(row - 1);
			const uchar* current = binary_image.ptr<uchar>(row);
			const uchar* next = binary_image.ptr<uchar>(row + 1);
			uchar* marker_pointer = marker.ptr<uchar>(row);

			for (int column = 1; column < binary_image.cols - 1; ++column)
			{
				const auto pi = current[column];

				if (pi == background)
				{
					continue;
				}

				const auto p1 = previous[column - 1];
				const auto p2 = previous[column];
				const auto p3 = previous[column + 1];
				const auto p4 = current[column + 1];
				const auto p5 = next[column + 1];
				const auto p6 = next[column];
				const auto p7 = next[column - 1];
				const auto p8 = current[column - 1];

				const auto a =
					(p1 == 0 && p2 == 1) +
					(p2 == 0 && p3 == 1) +
					(p3 == 0 && p4 == 1) +
					(p4 == 0 && p5 == 1) +
					(p5 == 0 && p6 == 1) +
					(p6 == 0 && p7 == 1) +
					(p7 == 0 && p8 == 1) +
					(p8 == 0 && p1 == 1);

				const auto b =
					p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8;

				const auto step_condition_c = p2 * p4 * p8;
				const auto step_condition_d = p2 * p6 * p8;

				if (a == 1 && b >= 2 && b <= 6 &&
					step_condition_c == 0 && step_condition_d == 0)
				{
					marker_pointer[column] = skeleton;
				}
			}
		}

		binary_image &= ~marker;
	}

	/**
	 * @brief Removes oblique corner artifacts after deletion passes.
	 *
	 * @param binary_image Binary image modified in place.
	 * @param marker Temporary marker matrix.
	 */
	void kwon_kang::cleanup_oblique_corners(cv::Mat& binary_image,
	                                        cv::Mat& marker)
	{
		marker.setTo(0);

		for (int row = 1; row < binary_image.rows - 1; ++row)
		{
			const uchar* prev = binary_image.ptr<uchar>(row - 1);
			const uchar* curr = binary_image.ptr<uchar>(row);
			const uchar* next = binary_image.ptr<uchar>(row + 1);
			uchar* marker_pointer = marker.ptr<uchar>(row);

			for (int col = 1; col < binary_image.cols - 1; ++col)
			{
				const auto pi = curr[col];

				if (pi == background)
				{
					continue;
				}

				const auto p1 = prev[col - 1];
				const auto p3 = prev[col + 1];
				const auto p4 = curr[col + 1];
				const auto p5 = next[col + 1];
				const auto p6 = next[col];
				const auto p7 = next[col - 1];
				const auto p8 = curr[col - 1];

				const auto condition_1 = (p3 == 0) && (p1 * p6 * p8 == 1);
				const auto condition_2 = (p1 == 0) && (p3 * p4 * p6 == 1);
				const auto condition_3 = (p3 == 0) && (p5 * p6 * p8 == 1);
				const auto condition_4 = (p1 == 0) && (p4 * p6 * p7 == 1);

				if (condition_1 || condition_2 || condition_3 || condition_4)
				{
					marker_pointer[col] = skeleton;
				}
			}
		}

		binary_image &= ~marker;
	}
}
