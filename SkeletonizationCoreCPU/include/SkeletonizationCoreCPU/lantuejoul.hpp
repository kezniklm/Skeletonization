/**
*
* @file lantuejoul.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares the CPU lantuejoul skeletonization algorithm.
*
* This header defines the CPU implementation interface for the lantuejoul
* morphological skeletonization procedure.
*
* Main responsibilities:
* - declare the CPU lantuejoul algorithm class
* - expose binary image skeletonization entry point
* - connect lantuejoul implementation to backend registration
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
	 * @class lantuejoul
	 * @brief Implements lantuejoul skeletonization on CPU.
	 *
	 * This class applies morphological operations to derive a skeleton from a
	 * binary foreground image.
	 */
	class lantuejoul final : public backend_cpu, public ::skeletonizer::algorithms::lantuejoul
	{
	public:
		/**
		 * @brief Applies lantuejoul skeletonization to a binary image.
		 *
		 * @param binary_image Binary image modified in place.
		 */
		void apply(cv::Mat& binary_image) const override;
	};
}
