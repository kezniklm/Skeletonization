/**
*
* @file guo_hall.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares the CPU guo-hall thinning algorithm.
*
* This header defines the CPU implementation interface for the guo-hall
* iterative thinning method.
*
* Main responsibilities:
* - declare the CPU guo-hall algorithm class
* - expose iterative thinning entry point
* - define first and second sub-iteration helpers
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
	 * @class guo_hall
	 * @brief Implements guo-hall thinning on CPU.
	 *
	 * This class applies alternating deletion passes until no foreground pixel
	 * changes between iterations.
	 */
	class guo_hall final : public backend_cpu, public ::skeletonizer::algorithms::guo_hall
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
		 * @brief Executes the first guo-hall deletion pass.
		 *
		 * @param binary_image Binary image modified in place.
		 * @param marker Temporary marker matrix.
		 */
		static void first_iteration(cv::Mat& binary_image, cv::Mat& marker);
		/**
		 * @brief Executes the second guo-hall deletion pass.
		 *
		 * @param binary_image Binary image modified in place.
		 * @param marker Temporary marker matrix.
		 */
		static void second_iteration(cv::Mat& binary_image, cv::Mat& marker);
	};
}
