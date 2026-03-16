/**
*
* @file tarabek.cpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Implements the multithreaded tarabek thinning algorithm.
*
* This file performs threaded tarabek deletion passes and postprocessing.
*
* Main responsibilities:
* - run threaded tarabek iterative loop
* - execute first and second parallel passes
* - apply postprocessing refinements
*
* @version 1.0
* @date 2026-03-16
*/

#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>

#include "SkeletonizationCoreMT/tarabek.hpp"

namespace skeletonizer::mt::algorithms
{
	/**
	 * @brief Applies threaded tarabek thinning.
	 *
	 * @param binary_image Binary image modified in place.
	 */
	void tarabek::apply(cv::Mat& binary_image) const
	{
		cv::Mat prev(binary_image.size(), CV_8UC1, cv::Scalar(0));
		cv::Mat marker(binary_image.size(), CV_8UC1);
		cv::Mat difference;

		do
		{
			first_iteration(binary_image, marker);
			second_iteration(binary_image, marker);

			cv::absdiff(binary_image, prev, difference);
			binary_image.copyTo(prev);
		}
		while (has_changed(difference));

		postprocessing(binary_image);

		clear_border(binary_image);
	}

	/**
	 * @brief Executes first parallel deletion pass.
	 *
	 * @param binary_image Binary image modified in place.
	 * @param marker Temporary marker matrix.
	 */
	void tarabek::first_iteration(cv::Mat& binary_image, cv::Mat& marker)
	{
		marker.setTo(0);

		cv::parallel_for_(cv::Range(1, binary_image.rows - 1),
		                  [&](const cv::Range& range)
		                  {
			                  for (int row = range.start; row < range.end; ++row)
			                  {
				                  const auto* previous = binary_image.ptr<uint8_t>(row - 1);
				                  const auto* current = binary_image.ptr<uint8_t>(row);
				                  const auto* next = binary_image.ptr<uint8_t>(row + 1);
				                  auto* marker_pointer = marker.ptr<uint8_t>(row);

				                  for (int column = 1; column < binary_image.cols - 1; ++column)
				                  {
					                  const auto p1 = current[column];

					                  if (p1 == background)
					                  {
						                  continue;
					                  }

					                  const auto p2 = previous[column];
					                  const auto p3 = previous[column + 1];
					                  const auto p4 = current[column + 1];
					                  const auto p5 = next[column + 1];
					                  const auto p6 = next[column];
					                  const auto p7 = next[column - 1];
					                  const auto p8 = current[column - 1];
					                  const auto p9 = previous[column - 1];

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

					                  if (a != 1 || b < 2 || b > 6 ||
						                  step_condition_c != 0 || step_condition_d != 0)
					                  {
						                  continue;
					                  }

					                  if (b == 2)
					                  {
						                  const auto bn4 = p2 + p4 + p6 + p8;

						                  const auto an4 =
							                  (p2 == 0 && p4 == 1) +
							                  (p4 == 0 && p6 == 1) +
							                  (p6 == 0 && p8 == 1) +
							                  (p8 == 0 && p2 == 1);

						                  const auto bnd = p3 + p5 + p7 + p9;

						                  const auto AND =
							                  (p3 == 0 && p5 == 1) +
							                  (p5 == 0 && p7 == 1) +
							                  (p7 == 0 && p9 == 1) +
							                  (p9 == 0 && p3 == 1);

						                  if (bn4 == 3 && an4 == 2 && bnd == 4 && AND == 2)
						                  {
							                  continue;
						                  }
					                  }

					                  if (b == 3 && a == 1 &&
						                  p4 == foreground &&
						                  p5 == foreground &&
						                  p6 == foreground)
					                  {
						                  continue;
					                  }

					                  marker_pointer[column] = foreground;
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
	void tarabek::second_iteration(cv::Mat& binary_image, cv::Mat& marker)
	{
		marker.setTo(0);

		cv::parallel_for_(cv::Range(1, binary_image.rows - 1),
		                  [&](const cv::Range& range)
		                  {
			                  for (int row = range.start; row < range.end; ++row)
			                  {
				                  const auto* previous = binary_image.ptr<uint8_t>(row - 1);
				                  const auto* current = binary_image.ptr<uint8_t>(row);
				                  const auto* next = binary_image.ptr<uint8_t>(row + 1);
				                  auto* marker_pointer = marker.ptr<uint8_t>(row);

				                  for (int column = 1; column < binary_image.cols - 1; ++column)
				                  {
					                  const auto p1 = current[column];

					                  if (p1 == background)
					                  {
						                  continue;
					                  }

					                  const auto p2 = previous[column];
					                  const auto p3 = previous[column + 1];
					                  const auto p4 = current[column + 1];
					                  const auto p5 = next[column + 1];
					                  const auto p6 = next[column];
					                  const auto p7 = next[column - 1];
					                  const auto p8 = current[column - 1];
					                  const auto p9 = previous[column - 1];

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

					                  if (a != 1 || b < 2 || b > 6 ||
						                  step_condition_c != 0 || step_condition_d != 0)
					                  {
						                  continue;
					                  }

					                  if (b == 2)
					                  {
						                  const auto bn4 = p2 + p4 + p6 + p8;

						                  const auto an4 =
							                  (p2 == 0 && p4 == 1) +
							                  (p4 == 0 && p6 == 1) +
							                  (p6 == 0 && p8 == 1) +
							                  (p8 == 0 && p2 == 1);

						                  const auto bnd = p3 + p5 + p7 + p9;

						                  const auto AND =
							                  (p3 == 0 && p5 == 1) +
							                  (p5 == 0 && p7 == 1) +
							                  (p7 == 0 && p9 == 1) +
							                  (p9 == 0 && p3 == 1);

						                  if (bn4 == 3 && an4 == 2 && bnd == 4 && AND == 2)
						                  {
							                  continue;
						                  }
					                  }

					                  if (b == 3 && a == 1 &&
						                  p4 == foreground &&
						                  p5 == foreground &&
						                  p6 == foreground)
					                  {
						                  continue;
					                  }

					                  marker_pointer[column] = foreground;
				                  }
			                  }
		                  });

		binary_image &= ~marker;
	}

	/**
	 * @brief Applies tarabek postprocessing refinements.
	 *
	 * @param binary_image Binary image modified in place.
	 */
	void tarabek::postprocessing(cv::Mat& binary_image)
	{
		process_template_set(binary_image, templates_a_bits, background);
		process_template_set(binary_image, templates_b_bits, background);
	}
}
