/**
*
* @file choi_lam_siu.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares the CPU choi-lam-siu thinning algorithm.
*
* This header defines the CPU implementation interface for choi-lam-siu
* thinning, including distance map and neighborhood helper declarations.
*
* Main responsibilities:
* - declare the CPU choi-lam-siu algorithm class
* - define distance-map construction helper declarations
* - expose neighborhood validation helper declarations
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
	 * @class choi_lam_siu
	 * @brief Implements choi-lam-siu thinning on CPU.
	 *
	 * This class computes distance-map constrained deletions for foreground
	 * pixels based on local neighborhood checks.
	 */
	class choi_lam_siu final : public backend_cpu, public ::skeletonizer::algorithms::choi_lam_siu
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
		 * @brief Performs skeletonization with residual-distance constraints.
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
		 * @brief Computes nearest-background distance maps.
		 *
		 * @param binary_image Input binary image.
		 * @return Distance map pair for rows and columns.
		 */
		static xy_distance_maps get_distance_map(const cv::Mat& binary_image);

		/**
		 * @brief Builds label-to-background-point lookup table.
		 *
		 * @param binary_image Input binary image.
		 * @param label_matrix Label matrix from distance transform.
		 * @return Lookup table of nearest background points.
		 */
		static std::vector<cv::Point> build_label_to_background_point_lut(
			const cv::Mat& binary_image,
			const cv::Mat& label_matrix);

		/**
		 * @brief Fills row and column offset maps.
		 *
		 * @param binary_image Input binary image.
		 * @param labels Label matrix from distance transform.
		 * @param label_to_background_point_lut Label lookup table.
		 * @return Tuple containing row and column offset maps.
		 */
		static std::tuple<cv::Mat, cv::Mat> fill_offset_maps(
			const cv::Mat& binary_image,
			const cv::Mat& labels,
			const std::vector<cv::Point>& label_to_background_point_lut);

		/**
		 * @brief Checks neighborhood residual-distance constraints.
		 *
		 * @param dy_map Row offset map.
		 * @param dx_map Column offset map.
		 * @param y Row index.
		 * @param x Column index.
		 * @param min_d2 Minimum squared distance.
		 * @param max_d2 Maximum squared distance.
		 * @return True when neighborhood satisfies constraints.
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
