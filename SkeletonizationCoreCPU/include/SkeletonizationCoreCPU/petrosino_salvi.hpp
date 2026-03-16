/**
*
* @file petrosino_salvi.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares the CPU petrosino-salvi thinning algorithm.
*
* This header defines the CPU implementation interface for the
* petrosino-salvi iterative thinning method.
*
* Main responsibilities:
* - declare the CPU petrosino-salvi algorithm class
* - expose iterative thinning entry point
* - define first and second pass helper declarations
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <opencv2/core.hpp>

#include "SkeletonizationCore/skeletonizer/backend.hpp"
#include "SkeletonizationCore/skeletonizer/algorithms.hpp"

namespace skeletonizer::cpu::algorithms
{
	/**
	 * @class petrosino_salvi
	 * @brief Implements petrosino-salvi thinning on CPU.
	 *
	 * This class applies two iterative deletion passes until the binary image
	 * converges to a stable skeleton.
	 */
	class petrosino_salvi final : public backend_cpu, public ::skeletonizer::algorithms::petrosino_salvi
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
