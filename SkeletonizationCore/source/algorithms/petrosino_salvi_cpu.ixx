module;

#include <opencv2/core.hpp>

export module skeletonizer_cpu:petrosino_salvi;

import :core;
import image_processing;

export namespace skeletonizer::cpu::algorithms
{
	class petrosino_salvi_cpu final : public skeletonizer_cpu, public ::skeletonizer::algorithms::petrosino_salvi
	{
		void apply(cv::Mat& binary_image) const override
		{
			cv::Mat previous(binary_image.size(), CV_8UC1, cv::Scalar(0));

			cv::Mat marker(binary_image.size(), CV_8UC1);

			cv::Mat difference;

			do
			{
				first_iteration(binary_image, marker);
				second_iteration(binary_image, marker);

				cv::absdiff(binary_image, previous, difference);
				binary_image.copyTo(previous);
			}
			while (has_changed(difference));

			clear_border(binary_image);
		}

		static void first_iteration(cv::Mat& binary_image, cv::Mat& marker)
		{
			marker.setTo(0);

			for (auto row = 2; row < binary_image.rows - 2; row++)
			{
				const uchar* prev1 = binary_image.ptr<uchar>(row - 1);
				const uchar* curr = binary_image.ptr<uchar>(row);
				const uchar* next1 = binary_image.ptr<uchar>(row + 1);
				const uchar* next2 = binary_image.ptr<uchar>(row + 2);
				uchar* mark_row = marker.ptr<uchar>(row);

				for (auto column = 2; column < binary_image.cols - 2; column++)
				{
					const auto x0 = curr[column];

					if (x0 == background)
					{
						continue;
					}

					const auto x1 = curr[column + 1];
					const auto x2 = prev1[column + 1];
					const auto x3 = prev1[column];
					const auto x4 = prev1[column - 1];
					const auto x5 = curr[column - 1];
					const auto x6 = next1[column - 1];
					const auto x7 = next1[column];
					const auto x8 = next1[column + 1];
					const auto y2 = next2[column];
					const auto y5 = curr[column + 2];

					const int a = ((x2 ^ x3) != 0) + ((x3 ^ x4) != 0) + ((x4 ^ x5) != 0) +
						((x5 ^ x6) != 0) + ((x6 ^ x7) != 0) + ((x7 ^ x8) != 0) +
						((x8 ^ x1) != 0) + ((x1 ^ x2) != 0);

					const int b = (x2 != 0) + (x3 != 0) + (x4 != 0) + (x5 != 0) +
						(x6 != 0) + (x7 != 0) + (x8 != 0) + (x1 != 0);

					const int r = x1 && x7 && x8 &&
						((!y5 && x2 && x3 && !x5) || (!y2 && !x3 && x5 && x6));

					if (a == 2 && b >= 2 && b <= 6 && r == 0)
					{
						mark_row[column] = skeleton;
					}
				}
			}

			binary_image &= ~marker;
		}

		static void second_iteration(cv::Mat& binary_image, cv::Mat& marker)
		{
			marker.setTo(0);

			for (auto row = 2; row < binary_image.rows - 2; row++)
			{
				const uchar* prev1 = binary_image.ptr<uchar>(row - 1);
				const uchar* curr = binary_image.ptr<uchar>(row);
				const uchar* next1 = binary_image.ptr<uchar>(row + 1);

				uchar* mark_row = marker.ptr<uchar>(row);

				for (auto column = 2; column < binary_image.cols - 2; column++)
				{
					const auto x0 = curr[column];

					if (x0 == background)
					{
						continue;
					}

					const auto x1 = curr[column + 1];
					const auto x2 = prev1[column + 1];
					const auto x3 = prev1[column];
					const auto x4 = prev1[column - 1];
					const auto x5 = curr[column - 1];
					const auto x6 = next1[column - 1];
					const auto x7 = next1[column];
					const auto x8 = next1[column + 1];

					const int s0 = (x3 && x7) || (x5 && x1);
					const int s1 = (x1 && !x6 && (!x4 || x3)) || (x3 && !x8 && (!x6 || x5)) ||
						(x7 && !x4 && (!x2 || x1)) || (x5 && !x2 && (!x8 || x7));

					const int b = (x2 != 0) + (x3 != 0) + (x4 != 0) + (x5 != 0) +
						(x6 != 0) + (x7 != 0) + (x8 != 0) + (x1 != 0);

					const int r = (x3 && ((x1 && !x8) || (x5 && !x6))) ||
						(x7 && ((!x5 && !x8) || (!x1 && !x6)));

					if (!s0 && s1 && r == 0 && b >= 3)
					{
						mark_row[column] = skeleton;
					}
				}
			}

			binary_image &= ~marker;
		}
	};
}
