/**
*
* @file guo_hal.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares the multithreaded guo-hall thinning algorithm.
*
* This header defines the threaded implementation interface for guo-hall
* thinning.
*
* Main responsibilities:
* - declare threaded guo-hall algorithm class
* - expose iterative thinning entry point
* - declare first and second iteration helpers
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
	 * @class guo_hall
	 * @brief Implements guo-hall thinning with threaded processing.
	 *
	 * This class runs guo-hall deletion passes in parallel row ranges.
	 */
	class guo_hall final : public backend_threads, public ::skeletonizer::algorithms::guo_hall
	{
	public:
		/**
		 * @brief Applies guo-hall thinning to a binary image.
		 *
		 * @param binary_image Binary image modified in place.
		 */
		void apply(cv::Mat& binary_image) const override;

	private:
		/**
		 * @brief Executes the first deletion pass.
		 *
		 * @param binary_image Binary image modified in place.
		 * @param marker Temporary marker matrix.
		 */
		static void first_iteration(cv::Mat& binary_image, cv::Mat& marker);
		/**
		 * @brief Executes the second deletion pass.
		 *
		 * @param binary_image Binary image modified in place.
		 * @param marker Temporary marker matrix.
		 */
		static void second_iteration(cv::Mat& binary_image, cv::Mat& marker);
	};
}
