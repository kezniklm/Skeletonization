/**
*
* @file han_la_rhee.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares the CPU han-la-rhee thinning algorithm.
*
* This header defines the CPU implementation interface for han-la-rhee
* thinning, including weight computation and iterative deletion.
*
* Main responsibilities:
* - declare the CPU han-la-rhee algorithm class
* - expose iterative thinning entry point
* - define weight computation and iteration helpers
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
	 * @class han_la_rhee
	 * @brief Implements han-la-rhee thinning on CPU.
	 *
	 * This class computes local weights and iteratively removes pixels according
	 * to han-la-rhee conditions.
	 */
	class han_la_rhee final : public backend_cpu, public ::skeletonizer::algorithms::han_la_rhee
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
		 * @brief Executes one weighted deletion iteration.
		 *
		 * @param binary_image Binary image modified in place.
		 * @param marker Temporary marker matrix.
		 * @param weight Per-pixel weight matrix.
		 */
		static void iteration(cv::Mat& binary_image, cv::Mat& marker, const cv::Mat& weight);

		/**
		 * @brief Calculates per-pixel neighborhood weights.
		 *
		 * @param image Binary image input.
		 * @param weight Output weight matrix.
		 */
		static void calculate_weight(cv::Mat& image, cv::Mat& weight);
	};
}
