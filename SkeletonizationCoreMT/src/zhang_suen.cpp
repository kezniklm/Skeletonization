/**
*
* @file zhang_suen.cpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Implements the multithreaded zhang-suen thinning algorithm.
*
* This file executes threaded zhang-suen deletion passes until convergence.
*
* Main responsibilities:
* - run threaded zhang-suen iterative loop
* - execute first and second parallel passes
* - clear output borders after convergence
*
* @version 1.0
* @date 2026-03-16
*/

#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>

#include "SkeletonizationCoreMT/zhang_suen.hpp"

namespace skeletonizer::mt::algorithms
{
	/**
	 * @brief Applies threaded zhang-suen thinning.
	 *
	 * @param binary_image Binary image modified in place.
	 */
	void zhang_suen::apply(cv::Mat& binary_image) const
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

		clear_border(binary_image);
	}

	/**
	 * @brief Executes first parallel deletion pass.
	 *
	 * @param binary_image Binary image modified in place.
	 * @param marker Temporary marker matrix.
	 */
	void zhang_suen::first_iteration(cv::Mat& binary_image, cv::Mat& marker)
	{
		marker.setTo(0);

		cv::parallel_for_(cv::Range(1, binary_image.rows - 1),
		                  [&](const cv::Range& range)
		                  {
			                  for (int row = range.start; row < range.end; ++row)
			                  {
				                  const uchar* previous = binary_image.ptr<uchar>(row - 1);
				                  const uchar* current = binary_image.ptr<uchar>(row);
				                  const uchar* next = binary_image.ptr<uchar>(row + 1);
				                  uchar* marker_pointer = marker.ptr<uchar>(row);

				                  for (int column = 1; column < binary_image.cols - 1; ++column)
				                  {
					                  const auto p1 = current[column];

					                  if (p1 == background)
					                  {
						                  continue;
					                  }

					                  const uchar p2 = previous[column];
					                  const uchar p3 = previous[column + 1];
					                  const uchar p4 = current[column + 1];
					                  const uchar p5 = next[column + 1];
					                  const uchar p6 = next[column];
					                  const uchar p7 = next[column - 1];
					                  const uchar p8 = current[column - 1];
					                  const uchar p9 = previous[column - 1];

					                  const int a =
						                  (p2 == 0 && p3 == 1) +
						                  (p3 == 0 && p4 == 1) +
						                  (p4 == 0 && p5 == 1) +
						                  (p5 == 0 && p6 == 1) +
						                  (p6 == 0 && p7 == 1) +
						                  (p7 == 0 && p8 == 1) +
						                  (p8 == 0 && p9 == 1) +
						                  (p9 == 0 && p2 == 1);

					                  const int b =
						                  p2 + p3 + p4 + p5 +
						                  p6 + p7 + p8 + p9;

					                  const int step_condition_c = p2 * p4 * p6;
					                  const int step_condition_d = p4 * p6 * p8;

					                  if (a == 1 &&
						                  b >= 2 && b <= 6 &&
						                  step_condition_c == 0 &&
						                  step_condition_d == 0)
					                  {
						                  marker_pointer[column] = skeleton;
					                  }
				                  }
			                  }
		                  });

		binary_image &= ~marker;
	}

	/**
	 * @brief Executes second parallel deletion pass.
	 *
	 * @param binary_image Binary image modified in place.
	 * @param marker Temporary marker matrix.
	 */
	void zhang_suen::second_iteration(cv::Mat& binary_image, cv::Mat& marker)
	{
		marker.setTo(0);

		cv::parallel_for_(cv::Range(1, binary_image.rows - 1),
		                  [&](const cv::Range& range)
		                  {
			                  for (int row = range.start; row < range.end; ++row)
			                  {
				                  const uchar* previous = binary_image.ptr<uchar>(row - 1);
				                  const uchar* current = binary_image.ptr<uchar>(row);
				                  const uchar* next = binary_image.ptr<uchar>(row + 1);
				                  uchar* marker_pointer = marker.ptr<uchar>(row);

				                  for (int column = 1; column < binary_image.cols - 1; ++column)
				                  {
					                  const uchar p1 = current[column];

					                  if (p1 == background)
					                  {
						                  continue;
					                  }

					                  const uchar p2 = previous[column];
					                  const uchar p3 = previous[column + 1];
					                  const uchar p4 = current[column + 1];
					                  const uchar p5 = next[column + 1];
					                  const uchar p6 = next[column];
					                  const uchar p7 = next[column - 1];
					                  const uchar p8 = current[column - 1];
					                  const uchar p9 = previous[column - 1];

					                  const int a =
						                  (p2 == 0 && p3 == 1) +
						                  (p3 == 0 && p4 == 1) +
						                  (p4 == 0 && p5 == 1) +
						                  (p5 == 0 && p6 == 1) +
						                  (p6 == 0 && p7 == 1) +
						                  (p7 == 0 && p8 == 1) +
						                  (p8 == 0 && p9 == 1) +
						                  (p9 == 0 && p2 == 1);

					                  const int b =
						                  p2 + p3 + p4 + p5 +
						                  p6 + p7 + p8 + p9;

					                  const int step_condition_c = p2 * p4 * p8;
					                  const int step_condition_d = p2 * p6 * p8;

					                  if (a == 1 &&
						                  b >= 2 && b <= 6 &&
						                  step_condition_c == 0 &&
						                  step_condition_d == 0)
					                  {
						                  marker_pointer[column] = skeleton;
					                  }
				                  }
			                  }
		                  });

		binary_image &= ~marker;
	}
}
