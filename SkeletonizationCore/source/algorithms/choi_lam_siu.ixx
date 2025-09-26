module;

#include <map>
#include <variant>

#include "opencv2/core.hpp"

export module skeletonizer_cpu:choi_lam_siu;

import :core;
import image_processing;

export namespace skeletonizer::cpu::algorithms
{
	class choi_lam_siu_cpu final : public skeletonizer_cpu,
	                               public ::skeletonizer::algorithms::choi_lam_siu
	{
	public:
		void apply(cv::Mat& binary_image) const override
		{
			const xy_distance_maps distance_maps = get_distance_map(binary_image);

			skeletonize(binary_image, distance_maps);

			clear_border(binary_image);
		}

		static inline void skeletonize(cv::Mat& binary_image, const xy_distance_maps& distance_maps,
		                               const int minimal_residual_distance = 100,
		                               const int maximal_residual_distance = INT_MAX)
		{
			for (auto row = 1; row < binary_image.rows - 1; ++row)
			{
				const auto binary_image_row_pointer = binary_image.ptr<uchar>(row);

				for (auto column = 1; column < binary_image.cols - 1; ++column)
				{
					if (!binary_image_row_pointer[column])
					{
						continue;
					}

					binary_image_row_pointer[column] = check_neighbourhood(
						                                   distance_maps.nearest_background_row_index,
						                                   distance_maps.nearest_background_column_index,
						                                   row, column, minimal_residual_distance,
						                                   maximal_residual_distance)
						                                   ? 1
						                                   : 0;
				}
			}
		}

		static inline xy_distance_maps get_distance_map(const cv::Mat& binary_image)
		{
			const auto labels = compute_nearest_background_labels(binary_image);

			const auto label_to_background_point_lut = build_label_to_background_point_lut(binary_image, labels);

			const auto [nearest_background_row_index, nearest_background_column_index] = fill_offset_maps(
				binary_image, labels, label_to_background_point_lut);

			return xy_distance_maps(std::move(nearest_background_row_index),
			                        std::move(nearest_background_column_index));
		}

		static inline std::vector<cv::Point> build_label_to_background_point_lut(const cv::Mat& binary_image,
			const cv::Mat& label_matrix)
		{
			const auto max_label_id = get_max_array_value(label_matrix);

			std::vector label_to_point(max_label_id + 1, cv::Point(-1, -1));

			for (auto y = 1; y < binary_image.rows - 1; ++y)
			{
				const auto image_row = binary_image.ptr<uchar>(y);

				const auto label_row = label_matrix.ptr<int>(y);

				for (auto x = 1; x < binary_image.cols - 1; ++x)
				{
					if (image_row[x] != 0) // background
					{
						continue;
					}

					const int label_id = label_row[x];

					if (label_id <= 0 || label_to_point[label_id].x >= 0)
					{
						continue;
					}

					label_to_point[label_id] = cv::Point(x, y); // first observed absolute coord for this label
				}
			}

			return label_to_point;
		}

		static inline std::tuple<cv::Mat, cv::Mat> fill_offset_maps(const cv::Mat& binary_image,
		                                                            const cv::Mat& labels,
		                                                            const std::vector<cv::Point>&
		                                                            label_to_background_point_lut)
		{
			auto dy = cv::Mat(binary_image.size(), CV_32SC1);
			auto dx = cv::Mat(binary_image.size(), CV_32SC1);

			const auto lut_size = static_cast<int>(label_to_background_point_lut.size());

			for (auto y = 1; y < binary_image.rows - 1; ++y)
			{
				const auto image_row = binary_image.ptr<uchar>(y);

				const int* label_row = labels.ptr<int>(y);

				const auto dy_row = dy.ptr<int>(y);
				const auto dx_row = dx.ptr<int>(y);

				for (int x = 1; x < binary_image.cols - 1; ++x)
				{
					if (image_row[x] == 0)
					{
						continue; // keep zeros for background
					}

					const auto label_id = label_row[x];

					if (label_id <= 0 || label_id >= lut_size || label_to_background_point_lut[label_id].x < 0)
					{
						continue;
					}

					const auto& q = label_to_background_point_lut[label_id]; // absolute nearest background pixel
					dx_row[x] = q.x - x; // Δx (columns)
					dy_row[x] = q.y - y; // Δy (rows)
				}
			}

			return {dy, dx};
		}

		static inline bool check_neighbourhood(
			const cv::Mat& dy_map, // = nearestBackgroundRowIndex = Qy - y
			const cv::Mat& dx_map, // = nearestBackgroundColumnIndex = Qx - x
			const int y,
			const int x,
			const int min_d2,
			const int max_d2)
		{
			// 8-neighborhood offsets (Δx, Δy) with center skipped
			static constexpr std::array<int, 8> d_x{{-1, 0, 1, -1, 1, -1, 0, 1}};
			static constexpr std::array<int, 8> d_y{{-1, -1, -1, 0, 0, 1, 1, 1}};

			constexpr std::size_t number_of_neighbours = d_x.size();

			static_assert(d_x.size() == d_y.size(), "d_x and d_y must have the same length");

			// Q = (qx, qy) relative to P
			const auto qx = dx_map.ptr<int>(y)[x];
			const auto qy = dy_map.ptr<int>(y)[x];

			const auto r2_q = squared_length(qx, qy);

			for (auto neighbour_index = 0; neighbour_index < number_of_neighbours; ++neighbour_index)
			{
				const auto nx = x + d_x[neighbour_index];
				const auto ny = y + d_y[neighbour_index];

				// Qi = DM(Pi) + (Δx,Δy)
				const auto qix = dx_map.ptr<int>(ny)[nx] + d_x[neighbour_index];
				const auto qiy = dy_map.ptr<int>(ny)[nx] + d_y[neighbour_index];

				const auto d2 = squared_length(qix - qx, qiy - qy);

				// Δr^2 = |Qi|^2 - |Q|^2
				const auto r2_qi = squared_length(qix, qiy);
				const auto delta_r2 = r2_qi - r2_q;

				const auto max_dir = std::max(std::abs(qx - qix), std::abs(qy - qiy)); // max(|Δx|,|Δy|)

				// CLS connectivity criterion (no division, no sqrt):
				// 1) D^2 within user [minD2, maxD2]
				// 2) r^2(Qi) - r^2(Q) <= maxInf
				const auto skeleton_width = static_cast<double>(delta_r2) / static_cast<double>(std::max(1, max_dir));

				if (d2 >= min_d2 && d2 <= max_d2 && skeleton_width <= 1.0)
				{
					return true;
				}
			}

			return false;
		}
	};
}
