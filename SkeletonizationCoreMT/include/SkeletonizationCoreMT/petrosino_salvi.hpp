/**
*
* @file petrosino_salvi.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares the multithreaded petrosino-salvi thinning algorithm.
*
* This header defines the threaded petrosino-salvi implementation interface.
*
* Main responsibilities:
* - declare threaded petrosino-salvi algorithm class
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
	 * @class petrosino_salvi
	 * @brief Implements petrosino-salvi thinning with threaded processing.
	 *
	 * This class runs two deletion passes in parallel row ranges.
	 */
	class petrosino_salvi final : public backend_threads, public ::skeletonizer::algorithms::petrosino_salvi
	{
	public:
		/**
		 * @brief Applies petrosino-salvi thinning to a binary image.
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
