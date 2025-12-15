#include <algorithm>
#include <array>
#include <atomic>
#include <cmath>

#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>

#include "SkeletonizationCoreMT/choi_lam_siu.hpp"

namespace skeletonizer::mt::algorithms
{
	void choi_lam_siu::apply(cv::Mat& binary_image) const
	{
		const xy_distance_maps distance_maps = get_distance_map(binary_image);

		skeletonize(binary_image, distance_maps);

		clear_border(binary_image);
	}

	void choi_lam_siu::skeletonize(const cv::Mat& binary_image,
	                               const xy_distance_maps& distance_maps,
	                               const int minimal_residual_distance,
	                               const int maximal_residual_distance)
	{
		auto skeleton_output = binary_image.clone();

		cv::parallel_for_(cv::Range(1, binary_image.rows - 1),
		                  [&](const cv::Range& range)
		                  {
			                  for (int row = range.start; row < range.end; ++row)
			                  {
				                  const auto* binary_image_row_ptr =
					                  binary_image.ptr<uchar>(row);
				                  auto* skeleton_output_row_ptr =
					                  skeleton_output.ptr<uchar>(row);

				                  for (int column = 1; column < binary_image.cols - 1; ++column)
				                  {
					                  if (!binary_image_row_ptr[column])
					                  {
						                  continue;
					                  }

					                  const bool keep =
						                  check_neighbourhood(
							                  distance_maps.nearest_background_row_index,
							                  distance_maps.nearest_background_column_index,
							                  row,
							                  column,
							                  minimal_residual_distance,
							                  maximal_residual_distance);

					                  skeleton_output_row_ptr[column] =
						                  keep ? skeleton : background;
				                  }
			                  }
		                  });

		skeleton_output.copyTo(binary_image);
	}

	choi_lam_siu::xy_distance_maps choi_lam_siu::get_distance_map(const cv::Mat& binary_image)
	{
		const auto labels =
			compute_nearest_background_labels(binary_image);

		const auto label_to_background_point_lut =
			build_label_to_background_point_lut(binary_image, labels);

		const auto [nearest_background_row_index,
				nearest_background_column_index] =
			fill_offset_maps(binary_image, labels, label_to_background_point_lut);

		return xy_distance_maps(std::move(nearest_background_row_index),
		                        std::move(nearest_background_column_index));
	}

	std::vector<cv::Point> choi_lam_siu::build_label_to_background_point_lut(
		const cv::Mat& binary_image,
		const cv::Mat& label_matrix)
	{
		const auto max_label_id = get_max_array_value(label_matrix);

		std::vector<cv::Point> label_to_point(max_label_id + 1,
		                                      cv::Point(-1, -1));

		std::vector<std::atomic<int>> claimed(max_label_id + 1);
		for (auto& a : claimed)
		{
			a.store(0, std::memory_order_relaxed);
		}

		cv::parallel_for_(cv::Range(1, binary_image.rows - 1),
		                  [&](const cv::Range& range)
		                  {
			                  for (int y = range.start; y < range.end; ++y)
			                  {
				                  const auto* image_row = binary_image.ptr<uchar>(y);
				                  const auto* label_row = label_matrix.ptr<int>(y);

				                  for (int x = 1; x < binary_image.cols - 1; ++x)
				                  {
					                  if (image_row[x] == foreground)
					                  {
						                  continue;
					                  }

					                  const int label_id = label_row[x];

					                  if (label_id <= 0 || label_to_point[label_id].x >= 0)
					                  {
						                  continue;
					                  }

					                  int expected = 0;
					                  if (claimed[label_id].compare_exchange_strong(
						                  expected, 1, std::memory_order_acq_rel))
					                  {
						                  label_to_point[label_id] = cv::Point(x, y);
					                  }
				                  }
			                  }
		                  });

		return label_to_point;
	}

	std::tuple<cv::Mat, cv::Mat> choi_lam_siu::fill_offset_maps(
		const cv::Mat& binary_image,
		const cv::Mat& labels,
		const std::vector<cv::Point>& label_to_background_point_lut)
	{
		cv::Mat dy(binary_image.size(), CV_32SC1, cv::Scalar(0));
		cv::Mat dx(binary_image.size(), CV_32SC1, cv::Scalar(0));

		const auto lut_size =
			static_cast<int>(label_to_background_point_lut.size());

		cv::parallel_for_(cv::Range(1, binary_image.rows - 1),
		                  [&](const cv::Range& range)
		                  {
			                  for (int y = range.start; y < range.end; ++y)
			                  {
				                  const auto* image_row = binary_image.ptr<uchar>(y);
				                  const int* label_row = labels.ptr<int>(y);
				                  auto* dy_row = dy.ptr<int>(y);
				                  auto* dx_row = dx.ptr<int>(y);

				                  for (int x = 1; x < binary_image.cols - 1; ++x)
				                  {
					                  if (image_row[x] == background)
					                  {
						                  continue;
					                  }

					                  const auto label_id = label_row[x];

					                  if (label_id <= 0 ||
						                  label_id >= lut_size ||
						                  label_to_background_point_lut[label_id].x < 0)
					                  {
						                  continue;
					                  }

					                  const auto& q = label_to_background_point_lut[label_id];
					                  dx_row[x] = q.x - x; // Δx
					                  dy_row[x] = q.y - y; // Δy
				                  }
			                  }
		                  });

		return {dy, dx};
	}

	bool choi_lam_siu::check_neighbourhood(
		const cv::Mat& dy_map,
		const cv::Mat& dx_map,
		const int y,
		const int x,
		const int min_d2,
		const int max_d2)
	{
		// 8-neighborhood offsets (Δx, Δy) with center skipped
		static constexpr std::array<int, 8> d_x{{-1, 0, 1, -1, 1, -1, 0, 1}};
		static constexpr std::array<int, 8> d_y{{-1, -1, -1, 0, 0, 1, 1, 1}};

		static_assert(d_x.size() == d_y.size(),
		              "d_x and d_y must have the same length");

		const int qx = dx_map.ptr<int>(y)[x];
		const int qy = dy_map.ptr<int>(y)[x];

		const auto r2_q = squared_length(qx, qy);

		for (std::size_t neighbour_index = 0;
		     neighbour_index < d_x.size();
		     ++neighbour_index)
		{
			const int nx = x + d_x[neighbour_index];
			const int ny = y + d_y[neighbour_index];

			const int qix =
				dx_map.ptr<int>(ny)[nx] + d_x[neighbour_index];
			const int qiy =
				dy_map.ptr<int>(ny)[nx] + d_y[neighbour_index];

			const auto d2 =
				squared_length(qix - qx, qiy - qy);

			const auto r2_qi = squared_length(qix, qiy);
			const auto delta_r2 = r2_qi - r2_q;

			const auto max_dir =
				std::max(std::abs(qx - qix), std::abs(qy - qiy));

			const auto skeleton_width =
				static_cast<double>(delta_r2) /
				static_cast<double>(std::max(1, max_dir));

			if (d2 >= min_d2 &&
				d2 <= max_d2 &&
				skeleton_width <= 1.0)
			{
				return true;
			}
		}

		return false;
	}
}
