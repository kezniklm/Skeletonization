/**
*
* @file zhang_suen.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares the CPU zhang-suen thinning algorithm.
*
* This header defines the CPU implementation interface for the zhang-suen
* iterative thinning method.
*
* Main responsibilities:
* - declare the CPU zhang-suen algorithm class
* - expose iterative thinning entry point
* - define internal sub-iteration processing helpers
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
	 * @class zhang_suen
	 * @brief Implements zhang-suen thinning on CPU.
	 *
	 * This class applies two alternating deletion passes until convergence.
	 */
	class zhang_suen final : public backend_cpu, public ::skeletonizer::algorithms::zhang_suen
	{
	public:
		/**
		 * @brief Applies zhang-suen thinning to a binary image.
		 *
		 * @param binary_image Binary image modified in place.
		 */
		void apply(cv::Mat& binary_image) const override;

	private:
		/**
		 * @brief Executes the first zhang-suen deletion pass.
		 *
		 * @param binary_image Binary image modified in place.
		 * @param marker Temporary marker matrix.
		 */
		static void first_iteration(cv::Mat& binary_image, cv::Mat& marker);
		/**
		 * @brief Executes the second zhang-suen deletion pass.
		 *
		 * @param binary_image Binary image modified in place.
		 * @param marker Temporary marker matrix.
		 */
		static void second_iteration(cv::Mat& binary_image, cv::Mat& marker);
	};
}
