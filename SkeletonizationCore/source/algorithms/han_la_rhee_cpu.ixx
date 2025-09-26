module;

#include <opencv2/core.hpp>

export module skeletonizer_cpu:han_la_rhee;

import :core;

export namespace skeletonizer::cpu::algorithms
{
	class han_la_rhee_cpu final : public skeletonizer_cpu, public ::skeletonizer::algorithms::han_la_rhee
	{
		void apply(cv::Mat& binary_image) const override
		{
			cv::Mat previous(binary_image.size(), CV_8UC1, cv::Scalar(0));
			cv::Mat marker(binary_image.size(), CV_8UC1);
			cv::Mat weight(binary_image.size(), CV_8SC1, cv::Scalar(0));
			cv::Mat difference;

			do
			{
				calculate_weight(binary_image, weight);
				iteration(binary_image, marker, weight);

				cv::absdiff(binary_image, previous, difference);
				binary_image.copyTo(previous);
			}
			while (has_changed(difference));

			clear_border(binary_image);
		}

		static void iteration(cv::Mat& binary_image, cv::Mat& marker, const cv::Mat& weight)
		{
			marker.setTo(0);

			for (int row = 1; row < binary_image.rows - 1; ++row)
			{
				const auto previous_weight = weight.ptr<uchar>(row - 1);
				const auto current_weight = weight.ptr<uchar>(row);
				const auto next_weight = weight.ptr<uchar>(row + 1);

				const auto marker_pointer = marker.ptr<uchar>(row);

				for (int col = 1; col < binary_image.cols - 1; ++col)
				{
					const auto xi = binary_image.ptr<uchar>(row)[col];

					if (xi != 1 || current_weight[col] <= 0 || current_weight[col] >= 8)
					{
						continue;
					}

					const auto x1 = previous_weight[col - 1];
					const auto x2 = previous_weight[col];
					const auto x3 = previous_weight[col + 1];
					const auto x4 = current_weight[col + 1];
					const auto x5 = next_weight[col + 1];
					const auto x6 = next_weight[col];
					const auto x7 = next_weight[col - 1];
					const auto x8 = current_weight[col - 1];

					const bool should_delete =
						(current_weight[col] == 1 && any_greater_or_equal_than(3, x1, x2, x3, x4, x5, x6, x7, x8)) ||
						(current_weight[col] == 2 && any_greater_or_equal_than(3, x1, x2, x3, x4, x5, x6, x7, x8) &&
							has_critical_pairs(x1, x2, x3, x4, x5, x6, x7, x8)) ||
						(current_weight[col] == 3 && any_greater_or_equal_than(7, x1, x2, x3, x4, x5, x6, x7, x8) &&
							(all_non_zero(x6, x7, x8) || all_non_zero(x1, x2, x3) ||
								all_non_zero(x1, x7, x8) || all_non_zero(x6, x7, x5) ||
								all_non_zero(x3, x4, x5) || all_non_zero(x2, x3, x4) ||
								all_non_zero(x4, x5, x6) || all_non_zero(x8, x1, x2) ||
								all_non_zero(x6, x7, x4) || all_non_zero(x6, x1, x8) ||
								all_non_zero(x6, x3, x4) || all_non_zero(x6, x5, x8))) ||
						(current_weight[col] == 4 && (all_non_zero(x1, x2, x3, x4) || all_non_zero(x1, x2, x7, x8) ||
							all_non_zero(x1, x2, x3, x8) || all_non_zero(x1, x6, x7, x8) ||
							all_non_zero(x5, x6, x7, x8) || all_non_zero(x4, x5, x6, x7) ||
							all_non_zero(x3, x4, x5, x6) || all_non_zero(x5, x2, x3, x4) ||
							all_non_zero(x6, x7, x3, x4) || all_non_zero(x1, x8, x5, x6))) ||
						(current_weight[col] == 5 && any_equal_to(8, x1, x2, x3, x4, x5, x6, x7, x8) &&
							(all_non_zero(x7, x8, x1, x2, x3) || all_non_zero(x7, x8, x1, x5, x6) ||
								all_non_zero(x3, x4, x5, x6, x7) || all_non_zero(x1, x2, x3, x4, x5) ||
								all_non_zero(x4, x5, x6, x7, x8) || all_non_zero(x6, x7, x8, x1, x2) ||
								all_non_zero(x1, x2, x3, x4, x8) || all_non_zero(x2, x3, x4, x5, x6))) ||
						(current_weight[col] == 6 && any_equal_to(8, x1, x2, x3, x4, x5, x6, x7, x8) &&
							(all_non_zero(x3, x4, x5, x6, x7, x8) || all_non_zero(x1, x2, x3, x6, x7, x8) ||
								all_non_zero(x1, x2, x5, x6, x7, x8) || all_non_zero(x1, x4, x5, x6, x7, x8) ||
								all_non_zero(x1, x2, x3, x4, x7, x8) || all_non_zero(x3, x4, x5, x6, x7, x2) ||
								all_non_zero(x3, x4, x5, x6, x1, x2) || all_non_zero(x1, x2, x3, x4, x5, x8))) ||
						(current_weight[col] == 7 && any_equal_to(8, x1, x2, x3, x4, x5, x6, x7, x8) &&
							(all_non_zero(x1, x2, x3, x5, x6, x7, x8) || all_non_zero(x1, x3, x4, x5, x6, x7, x8) ||
								all_non_zero(x1, x2, x3, x4, x5, x6, x7) || all_non_zero(x1, x2, x3, x4, x5, x7, x8)));

					if (should_delete)
					{
						marker_pointer[col] = 1;
					}
				}
			}

			binary_image &= ~marker;
		}

		static void calculate_weight(cv::Mat& image, cv::Mat& weight)
		{
			weight.setTo(0);

			for (auto row = 1; row < image.rows - 1; ++row)
			{
				const auto previous = image.ptr<uchar>(row - 1);
				const auto current = image.ptr<uchar>(row);
				const auto next = image.ptr<uchar>(row + 1);
				const auto weight_ptr = weight.ptr<uchar>(row);

				for (int col = 1; col < image.cols - 1; ++col)
				{
					const auto xi = current[col];

					if (xi != 1)
					{
						continue;
					}

					const auto x1 = previous[col - 1];
					const auto x2 = previous[col];
					const auto x3 = previous[col + 1];
					const auto x4 = current[col - 1];
					const auto x5 = current[col + 1];
					const auto x6 = next[col - 1];
					const auto x7 = next[col];
					const auto x8 = next[col + 1];

					weight_ptr[col] = x1 + x2 + x3 + x4 + x5 + x6 + x7 + x8;
				}
			}
		}
	};
}
