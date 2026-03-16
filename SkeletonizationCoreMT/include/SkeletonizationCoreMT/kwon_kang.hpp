/**
*
* @file kwon_kang.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares the multithreaded kwon-gi-kang thinning algorithm.
*
* This header defines the threaded implementation interface for kwon-gi-kang
* thinning.
*
* Main responsibilities:
* - declare threaded kwon-gi-kang algorithm class
* - expose iterative thinning entry point
* - declare pass and corner-cleanup helpers
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
	 * @class kwon_kang
	 * @brief Implements kwon-gi-kang thinning with threaded processing.
	 *
	 * This class runs two deletion passes and corner cleanup in parallel ranges.
	 */
	class kwon_kang final : public backend_threads, public ::skeletonizer::algorithms::kwon_kang
	{
	public:
		/**
		 * @brief Applies kwon-gi-kang thinning to a binary image.
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
		/**
		 * @brief Removes oblique corner artifacts.
		 *
		 * @param binary_image Binary image modified in place.
		 * @param marker Temporary marker matrix.
		 */
		static void cleanup_oblique_corners(cv::Mat& binary_image, cv::Mat& marker);
	};
}
