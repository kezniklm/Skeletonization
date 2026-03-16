/**
*
* @file choi_lam_siu.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares the multithreaded choi-lam-siu thinning algorithm.
*
* This header defines the threaded choi-lam-siu implementation interface and
* neighborhood helper declarations.
*
* Main responsibilities:
* - declare threaded choi-lam-siu algorithm class
* - expose distance-constrained skeletonization entry point
* - declare neighborhood and map helper functions
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <climits>
#include <tuple>
#include <vector>

#include <opencv2/core.hpp>

#include "SkeletonizationCore/skeletonizer/backend.hpp"
#include "SkeletonizationCore/skeletonizer/algorithms.hpp"

namespace skeletonizer::mt::algorithms
{
	/**
	 * @class choi_lam_siu
	 * @brief Implements choi-lam-siu thinning with threaded processing.
	 *
	 * This class computes distance maps and applies local residual checks.
	 */
	class choi_lam_siu final : public backend_threads, public ::skeletonizer::algorithms::choi_lam_siu
	{
	public:
		/**
		 * @brief Applies choi-lam-siu thinning to a binary image.
		 *
		 * @param binary_image Binary image modified in place.
		 */
		void apply(cv::Mat& binary_image) const override;

	private:
		/**
		 * @brief Performs distance-constrained skeletonization.
		 *
		 * @param binary_image Binary image modified in place.
		 * @param distance_maps Precomputed nearest-background maps.
		 * @param minimal_residual_distance Minimum residual distance.
		 * @param maximal_residual_distance Maximum residual distance.
		 */
		static void skeletonize(const cv::Mat& binary_image,
		                        const xy_distance_maps& distance_maps,
		                        int minimal_residual_distance = 100,
		                        int maximal_residual_distance = INT_MAX);

		/**
		 * @brief Computes nearest-background distance maps for each pixel.
		 * @param binary_image Input binary image.
		 * @return Pair of row/column offset maps.
		 */
		static xy_distance_maps get_distance_map(const cv::Mat& binary_image);

		/**
		 * @brief Builds label-to-background lookup table from label matrix.
		 * @param binary_image Input binary image.
		 * @param label_matrix Connected-component labels.
		 * @return Lookup table of nearest background points by label id.
		 */
		static std::vector<cv::Point> build_label_to_background_point_lut(
			const cv::Mat& binary_image,
			const cv::Mat& label_matrix);

		/**
		 * @brief Fills offset maps used by residual neighborhood checks.
		 * @param binary_image Input binary image.
		 * @param labels Connected-component labels.
		 * @param label_to_background_point_lut Label-to-point lookup table.
		 * @return Tuple of row and column offset maps.
		 */
		static std::tuple<cv::Mat, cv::Mat> fill_offset_maps(
			const cv::Mat& binary_image,
			const cv::Mat& labels,
			const std::vector<cv::Point>& label_to_background_point_lut);

		/**
		 * @brief Checks neighborhood residual constraints.
		 *
		 * @param dy_map Row offset map.
		 * @param dx_map Column offset map.
		 * @param y Row index.
		 * @param x Column index.
		 * @param min_d2 Minimum squared distance.
		 * @param max_d2 Maximum squared distance.
		 * @return True when neighborhood satisfies checks.
		 */
		static bool check_neighbourhood(
			const cv::Mat& dy_map, // nearestBackgroundRowIndex = Qy - y
			const cv::Mat& dx_map, // nearestBackgroundColumnIndex = Qx - x
			int y,
			int x,
			int min_d2,
			int max_d2);
	};
}
