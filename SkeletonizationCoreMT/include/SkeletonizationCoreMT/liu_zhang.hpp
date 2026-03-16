/**
*
* @file liu_zhang.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares the multithreaded liu-zhang thinning algorithm.
*
* This header defines threaded liu-zhang interfaces for constrained iteration
* and artifact cleanup.
*
* Main responsibilities:
* - declare threaded liu-zhang algorithm class
* - expose constrained first and second iteration helpers
* - declare ghij cleanup helper
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
	 * @class liu_zhang
	 * @brief Implements liu-zhang thinning with threaded processing.
	 *
	 * This class applies constrained passes and final pattern cleanup.
	 */
	class liu_zhang final : public backend_threads, public ::skeletonizer::algorithms::liu_zhang
	{
	public:
		/**
		 * @brief Applies liu-zhang thinning to a binary image.
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
		 * @param use_constraint Enables cp constraint.
		 * @return True when at least one pixel is deleted.
		 */
		static bool first_iteration(cv::Mat& binary_image,
		                            cv::Mat& marker,
		                            bool use_constraint);

		/**
		 * @brief Executes the second deletion pass.
		 *
		 * @param binary_image Binary image modified in place.
		 * @param marker Temporary marker matrix.
		 * @param use_constraint Enables cp constraint.
		 * @return True when at least one pixel is deleted.
		 */
		static bool second_iteration(cv::Mat& binary_image,
		                             cv::Mat& marker,
		                             bool use_constraint);

		/**
		 * @brief Deletes ghij pattern artifacts.
		 *
		 * @param binary_image Binary image modified in place.
		 * @param marker Temporary marker matrix.
		 */
		void delete_patterns_ghij(cv::Mat& binary_image,
		                          cv::Mat& marker) const;
	};
}
