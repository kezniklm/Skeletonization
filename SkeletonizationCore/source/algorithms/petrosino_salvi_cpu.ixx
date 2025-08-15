module;

#include <opencv2/core.hpp>

export module skeletonizer_cpu:petrosino_salvi;

import :core;
import image_processing;

namespace skeletonizer::cpu::algorithms
{
	export class petrosino_salvi_cpu final : public skeletonizer_cpu, public ::skeletonizer::algorithms::petrosino_salvi
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
		}

		static void first_iteration(cv::Mat& binary_image, cv::Mat& marker)
		{
			marker.setTo(0);

			for (int row = 2; row < binary_image.rows - 2; row++)
			{
				const uchar* prev1 = binary_image.ptr<uchar>(row - 1);
				const uchar* curr = binary_image.ptr<uchar>(row);
				const uchar* next1 = binary_image.ptr<uchar>(row + 1);
				const uchar* next2 = binary_image.ptr<uchar>(row + 2);
				uchar* mark_row = marker.ptr<uchar>(row);

				for (auto column = 2; column < binary_image.cols - 2; column++)
				{
					if (curr[column])
					{
						const uchar x1 = curr[column + 1];
						const uchar x2 = prev1[column + 1];
						const uchar x3 = prev1[column];
						const uchar x4 = prev1[column - 1];
						const uchar x5 = curr[column - 1];
						const uchar x6 = next1[column - 1];
						const uchar x7 = next1[column];
						const uchar x8 = next1[column + 1];
						const uchar y2 = next2[column];
						const uchar y5 = curr[column + 2];

						const int A = ((x2 ^ x3) != 0) + ((x3 ^ x4) != 0) + ((x4 ^ x5) != 0) +
							((x5 ^ x6) != 0) + ((x6 ^ x7) != 0) + ((x7 ^ x8) != 0) +
							((x8 ^ x1) != 0) + ((x1 ^ x2) != 0);

						const int B = (x2 != 0) + (x3 != 0) + (x4 != 0) + (x5 != 0) +
							(x6 != 0) + (x7 != 0) + (x8 != 0) + (x1 != 0);

						const int R = x1 && x7 && x8 &&
							((!y5 && x2 && x3 && !x5) || (!y2 && !x3 && x5 && x6));

						if (A == 2 && B >= 2 && B <= 6 && R == 0)
						{
							mark_row[column] = high;
						}
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
					if (curr[column])
					{
						const uchar x1 = curr[column + 1];
						const uchar x2 = prev1[column + 1];
						const uchar x3 = prev1[column];
						const uchar x4 = prev1[column - 1];
						const uchar x5 = curr[column - 1];
						const uchar x6 = next1[column - 1];
						const uchar x7 = next1[column];
						const uchar x8 = next1[column + 1];

						const int S0 = (x3 && x7) || (x5 && x1);
						const int S1 = (x1 && !x6 && (!x4 || x3)) || (x3 && !x8 && (!x6 || x5)) ||
							(x7 && !x4 && (!x2 || x1)) || (x5 && !x2 && (!x8 || x7));

						const int B = (x2 != 0) + (x3 != 0) + (x4 != 0) + (x5 != 0) +
							(x6 != 0) + (x7 != 0) + (x8 != 0) + (x1 != 0);

						const int R = (x3 && ((x1 && !x8) || (x5 && !x6))) ||
							(x7 && ((!x5 && !x8) || (!x1 && !x6)));

						if (!S0 && S1 && R == 0 && B >= 3)
						{
							mark_row[column] = 255;
						}
					}
				}
			}

			binary_image &= ~marker;
		}
	};
}
