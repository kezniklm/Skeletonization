#pragma once

#include <climits>
#include <tuple>
#include <vector>

#include <opencv2/core.hpp>

#include "SkeletonizationCore/skeletonizer/backend.hpp"
#include "SkeletonizationCore/skeletonizer/algorithms.hpp"

namespace skeletonizer::mt::algorithms
{
	class choi_lam_siu final : public backend_threads, public ::skeletonizer::algorithms::choi_lam_siu
	{
	public:
		void apply(cv::Mat& binary_image) const override;

	private:
		static void skeletonize(const cv::Mat& binary_image,
		                        const xy_distance_maps& distance_maps,
		                        int minimal_residual_distance = 100,
		                        int maximal_residual_distance = INT_MAX);

		static xy_distance_maps get_distance_map(const cv::Mat& binary_image);

		static std::vector<cv::Point> build_label_to_background_point_lut(
			const cv::Mat& binary_image,
			const cv::Mat& label_matrix);

		static std::tuple<cv::Mat, cv::Mat> fill_offset_maps(
			const cv::Mat& binary_image,
			const cv::Mat& labels,
			const std::vector<cv::Point>& label_to_background_point_lut);

		static bool check_neighbourhood(
			const cv::Mat& dy_map, // nearestBackgroundRowIndex = Qy - y
			const cv::Mat& dx_map, // nearestBackgroundColumnIndex = Qx - x
			int y,
			int x,
			int min_d2,
			int max_d2);
	};
}
