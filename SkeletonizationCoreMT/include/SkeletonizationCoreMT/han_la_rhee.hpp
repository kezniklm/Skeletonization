/**
*
* @file han_la_rhee.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares the multithreaded han-la-rhee thinning algorithm.
*
* This header defines threaded han-la-rhee interfaces for weighted iterative
* deletion.
*
* Main responsibilities:
* - declare threaded han-la-rhee algorithm class
* - expose weighted thinning entry point
* - declare iteration and weight-computation helpers
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <opencv2/core.hpp>

#include "SkeletonizationCore/skeletonizer/backend.hpp"
#include "SkeletonizationCore/skeletonizer/algorithms.hpp"

namespace skeletonizer::mt::algorithms
{
	/**
	 * @class han_la_rhee
	 * @brief Implements han-la-rhee thinning with threaded processing.
	 *
	 * This class computes weights and applies deletion in parallel row ranges.
	 */
	class han_la_rhee final : public backend_threads, public ::skeletonizer::algorithms::han_la_rhee
	{
	public:
		/**
		 * @brief Applies han-la-rhee thinning to a binary image.
		 *
		 * @param binary_image Binary image modified in place.
		 */
		void apply(cv::Mat& binary_image) const override;

	private:
		/**
		 * @brief Executes one deletion iteration.
		 *
		 * @param binary_image Source binary image.
		 * @param marker Temporary marker matrix.
		 * @param weight Weight matrix.
		 */
		static void iteration(const cv::Mat& binary_image,
		                      cv::Mat& marker,
		                      const cv::Mat& weight);

		/**
		 * @brief Calculates neighborhood weights for all pixels.
		 *
		 * @param image Binary image input.
		 * @param weight Output weight matrix.
		 */
		static void calculate_weight(cv::Mat& image, cv::Mat& weight);
	};
}
