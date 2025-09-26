module;

#include <opencv2/core.hpp>

export module skeletonizer_cpu:hesselink_roerdink_threads;

import :core;
import image_processing;

export namespace skeletonizer::cpu::algorithms
{
	class hesselink_roerdink_cpu_threads final : public skeletonizer_cpu,
	                                             public ::skeletonizer::algorithms::hesselink_roerdink
	{
		void apply(cv::Mat& binary_image) const override
		{
			binary_image *= high;

			cv::Mat skeleton_map = cv::Mat::zeros(binary_image.size(), CV_8UC1);

			const auto horizontal_feature_map = compute_horizontal_feature_map(binary_image);
			const auto vertical_feature_map = compute_vertical_feature_map(binary_image, horizontal_feature_map);

			apply_pruning_to_extract_skeleton(binary_image, vertical_feature_map, skeleton_map);

			binary_image = skeleton_map;
		}

		static constexpr auto pixel_background = 0;
		static constexpr auto pixel_skeleton = 128;
		static constexpr auto pruning_threshold = 5.0;

		static inline bool is_pixel_background(const cv::Mat& image, const int x, const int y)
		{
			return image.at<uchar>(y, x) == pixel_background;
		}

		static inline void set_skeleton_pixel(cv::Mat& image, const int x, const int y, const int value)
		{
			image.at<uchar>(y, x) = static_cast<uchar>(value);
		}

		static cv::Mat compute_horizontal_feature_map(const cv::Mat& binary_image)
		{
			const int max_distance = 1 + static_cast<int>(std::sqrt(
				binary_image.cols * binary_image.cols + binary_image.rows * binary_image.rows));

			cv::Mat horizontal_distance_map(binary_image.rows, binary_image.cols, CV_32SC1);
			cv::Mat horizontal_feature_map(binary_image.rows, binary_image.cols, CV_32SC1);

			cv::parallel_for_(cv::Range(0, binary_image.rows), [&](const cv::Range& range)
			{
				for (int row = range.start; row < range.end; ++row)
				{
					const auto distance_map_row = horizontal_distance_map.ptr<int>(row);
					const auto binary_image_row = binary_image.ptr<uchar>(row);

					distance_map_row[binary_image.cols - 1] = binary_image_row[binary_image.cols - 1] ==
					                                          pixel_background
						                                          ? 0
						                                          : max_distance;

					for (int column = binary_image.cols - 2; column >= 0; --column)
					{
						distance_map_row[column] = binary_image_row[column] == pixel_background
							                           ? 0
							                           : distance_map_row[column + 1] + 1;
					}

					const auto horizontal_feature_row = horizontal_feature_map.ptr<int>(row);
					horizontal_feature_row[0] = distance_map_row[0];

					for (int column = 1; column < binary_image.cols; ++column)
					{
						const int previous = horizontal_feature_row[column - 1];
						const int current = distance_map_row[column];
						horizontal_feature_row[column] = column - previous <= current ? previous : column + current;
					}
				}
			});

			return horizontal_feature_map;
		}

		static cv::Mat compute_vertical_feature_map(const cv::Mat& binary_image, const cv::Mat& horizontal_feature_map)
		{
			cv::Mat vertical_feature_map(binary_image.rows, binary_image.cols, CV_32SC1);

			cv::parallel_for_(cv::Range(0, binary_image.cols), [&](const cv::Range& range)
			{
				std::vector<int> feature_values_in_column(binary_image.rows);
				std::vector<int> squared_distances_to_boundary(binary_image.rows);
				std::vector<int> candidate_positions(binary_image.rows);
				std::vector<int> boundary_positions(binary_image.rows + 1);

				for (int column = range.start; column < range.end; ++column)
				{
					for (int row = 0; row < binary_image.rows; ++row)
					{
						const int horizontal_feature = horizontal_feature_map.at<int>(row, column);
						feature_values_in_column[row] = horizontal_feature;

						const int distance = horizontal_feature - column;
						squared_distances_to_boundary[row] = distance * distance;
					}

					int last_candidate_index = 0;
					candidate_positions[0] = 0;
					boundary_positions[0] = 0;

					for (int current_row = 1; current_row < binary_image.rows; ++current_row)
					{
						while (last_candidate_index >= 0 &&
							(current_row - candidate_positions[last_candidate_index]) *
							(current_row + candidate_positions[last_candidate_index] - 2 * boundary_positions[
								last_candidate_index]) <
							squared_distances_to_boundary[candidate_positions[last_candidate_index]] -
							squared_distances_to_boundary[current_row])
						{
							--last_candidate_index;
						}

						if (last_candidate_index < 0)
						{
							last_candidate_index = 0;
							candidate_positions[0] = current_row;
						}
						else
						{
							const int numerator = (current_row + candidate_positions[last_candidate_index]) *
								(current_row - candidate_positions[last_candidate_index]) +
								squared_distances_to_boundary[current_row] -
								squared_distances_to_boundary[candidate_positions[last_candidate_index]];

							const int denominator = 2 * (current_row - candidate_positions[last_candidate_index]);
							const int new_boundary = 1 + numerator / denominator;

							if (new_boundary < binary_image.rows)
							{
								++last_candidate_index;
								candidate_positions[last_candidate_index] = current_row;
								boundary_positions[last_candidate_index] = new_boundary;
							}
						}
					}

					for (int current_row = binary_image.rows - 1; current_row >= 0; --current_row)
					{
						const int chosen_candidate = candidate_positions[last_candidate_index];
						vertical_feature_map.at<int>(current_row, column) =
							feature_values_in_column[chosen_candidate] * binary_image.rows + chosen_candidate;

						if (current_row == boundary_positions[last_candidate_index])
						{
							--last_candidate_index;
						}
					}
				}
			});

			return vertical_feature_map;
		}

		static void apply_pruning_to_extract_skeleton(const cv::Mat& binary_image, const cv::Mat& vertical_feature_map,
		                                              cv::Mat& output)
		{
			cv::parallel_for_(cv::Range(1, binary_image.rows), [&](const cv::Range& range)
			{
				for (int row = range.start; row < range.end; ++row)
				{
					for (int column = 0; column < binary_image.cols; ++column)
					{
						if (column > 0 &&
							(!is_pixel_background(binary_image, column, row) || !is_pixel_background(
								binary_image, column - 1, row)))
						{
							compare_and_mark_skeleton_pixels(column, row, column - 1, row,
							                                 vertical_feature_map.at<int>(row, column),
							                                 vertical_feature_map.at<int>(row, column - 1),
							                                 binary_image.rows, output);
						}
						if (row > 0 &&
							(!is_pixel_background(binary_image, column, row) || !is_pixel_background(
								binary_image, column, row - 1)))
						{
							compare_and_mark_skeleton_pixels(column, row, column, row - 1,
							                                 vertical_feature_map.at<int>(row, column),
							                                 vertical_feature_map.at<int>(row - 1, column),
							                                 binary_image.rows, output);
						}
					}
				}
			});
		}

		static void compare_and_mark_skeleton_pixels(const int x, const int y, const int px, const int py,
		                                             const int f1, const int f2,
		                                             const int horizontal_limit, cv::Mat& img)
		{
			const int yf1 = f1 % horizontal_limit;
			const int xf1 = f1 / horizontal_limit;
			const int yf2 = f2 % horizontal_limit;
			const int xf2 = f2 / horizontal_limit;

			const int dx = xf1 - xf2;
			const int dy = yf1 - yf2;
			const int d2 = dx * dx + dy * dy;

			if (d2 <= 1)
			{
				return;
			}

			if (exceeds_pruning_threshold(d2, pruning_threshold, x, y, px, py, xf1, yf1, xf2, yf2, dx, dy))
			{
				const auto dot = dx * (xf1 + xf2 - x - px) + dy * (yf1 + yf2 - y - py);

				if (dot >= 0)
				{
					set_skeleton_pixel(img, x, y, pixel_skeleton);
				}

				if (dot <= 0)
				{
					set_skeleton_pixel(img, px, py, pixel_skeleton);
				}
			}
		}

		static bool exceeds_pruning_threshold(
			const float d2,
			const float pruning_threshold,
			const float x, const float y,
			const float px, const float py,
			const float xf1, const float yf1,
			const float xf2, const float yf2,
			const float dx, const float dy)
		{
			const float dx_total = x - xf1 + px - xf2;
			const float dy_total = y - yf1 + py - yf2;
			const float offset_dot = (x - px) * dx + (y - py) * dy;

			if (pruning_threshold > 0.0f)
			{
				const float threshold_sq = pruning_threshold * pruning_threshold;
				return d2 > threshold_sq;
			}

			if (pruning_threshold < 0.0f)
			{
				const float dist_sq = dx_total * dx_total + dy_total * dy_total;
				const float val = 1.0f + (dist_sq * pruning_threshold * pruning_threshold) / 4.0f;
				return d2 > val;
			}

			const double euclidean_dist = std::sqrt(dx_total * dx_total + dy_total * dy_total);
			const double dynamic_threshold = euclidean_dist + 2.0 * offset_dot + 1.0;

			return d2 > dynamic_threshold;
		}
	};
}
