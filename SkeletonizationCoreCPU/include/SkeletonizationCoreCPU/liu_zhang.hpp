/**
*
* @file liu_zhang.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares the CPU liu-zhang thinning algorithm.
*
* This header defines the CPU implementation interface for liu-zhang
* thinning, including constrained iterations and pattern deletion helpers.
*
* Main responsibilities:
* - declare the CPU liu-zhang algorithm class
* - expose constrained first and second iteration helpers
* - define pattern-specific cleanup helper declarations
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
	 * @class liu_zhang
	 * @brief Implements liu-zhang thinning on CPU.
	 *
	 * This class applies constrained two-phase deletion and optional pattern
	 * cleanup to preserve connectivity.
	 */
	class liu_zhang final : public backend_cpu, public ::skeletonizer::algorithms::liu_zhang
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
		 * @brief Executes the first liu-zhang deletion pass.
		 *
		 * @param binary_image Binary image modified in place.
		 * @param marker Temporary marker matrix.
		 * @param use_constraint Enables additional deletion constraints.
		 * @return True when at least one pixel is marked.
		 */
		static bool first_iteration(cv::Mat& binary_image,
		                            cv::Mat& marker,
		                            bool use_constraint);

		/**
		 * @brief Executes the second liu-zhang deletion pass.
		 *
		 * @param binary_image Binary image modified in place.
		 * @param marker Temporary marker matrix.
		 * @param use_constraint Enables additional deletion constraints.
		 * @return True when at least one pixel is marked.
		 */
		static bool second_iteration(cv::Mat& binary_image,
		                             cv::Mat& marker,
		                             bool use_constraint);

		/**
		 * @brief Deletes ghij pattern artifacts after main passes.
		 *
		 * @param binary_image Binary image modified in place.
		 * @param marker Temporary marker matrix.
		 */
		void delete_patterns_ghij(cv::Mat& binary_image,
		                          cv::Mat& marker) const;
	};
}
