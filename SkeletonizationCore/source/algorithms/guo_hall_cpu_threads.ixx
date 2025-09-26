module;

#include <opencv2/core.hpp>

export module skeletonizer_cpu:guo_hall_threads;

import :core;

export namespace skeletonizer::cpu::algorithms
{
	class guo_hall_cpu_threads final : public skeletonizer_cpu, public ::skeletonizer::algorithms::guo_hall
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

			cv::parallel_for_(cv::Range(1, binary_image.rows - 1), [&](const cv::Range& range)
			{
				for (int row = range.start; row < range.end; ++row)
				{
					const uchar* previous = binary_image.ptr<uchar>(row - 1);
					const uchar* current = binary_image.ptr<uchar>(row);
					const uchar* next = binary_image.ptr<uchar>(row + 1);
					uchar* marker_pointer = marker.ptr<uchar>(row);

					for (int column = 1; column < binary_image.cols - 1; ++column)
					{
						const uchar p1 = current[column];

						if (p1 != 1)
						{
							continue;
						}

						const uchar p2 = previous[column];
						const uchar p3 = previous[column + 1];
						const uchar p4 = current[column + 1];
						const uchar p5 = next[column + 1];
						const uchar p6 = next[column];
						const uchar p7 = next[column - 1];
						const uchar p8 = current[column - 1];
						const uchar p9 = previous[column - 1];

						const int c = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) +
							(p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) +
							(p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
							(p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);

						const int n1 = (p9 | p2) + (p3 | p4) + (p5 | p6) + (p7 | p8);
						const int n2 = (p2 | p3) + (p4 | p5) + (p6 | p7) + (p8 | p9);
						const int n = std::min(n1, n2);

						const int m = (p6 | p7 | !p9) & p8;

						if (c == 1 && (n >= 2 && n <= 3) && m == 0)
						{
							marker_pointer[column] = 1;
						}
					}
				}
			});

			binary_image &= ~marker;
		}

		static void second_iteration(cv::Mat& binary_image, cv::Mat& marker)
		{
			marker.setTo(0);

			cv::parallel_for_(cv::Range(1, binary_image.rows - 1), [&](const cv::Range& range)
			{
				for (int row = range.start; row < range.end; ++row)
				{
					const uchar* previous = binary_image.ptr<uchar>(row - 1);
					const uchar* current = binary_image.ptr<uchar>(row);
					const uchar* next = binary_image.ptr<uchar>(row + 1);
					uchar* marker_pointer = marker.ptr<uchar>(row);

					for (int column = 1; column < binary_image.cols - 1; ++column)
					{
						const uchar p1 = current[column];

						if (p1 != 1)
						{
							continue;
						}

						const uchar p2 = previous[column];
						const uchar p3 = previous[column + 1];
						const uchar p4 = current[column + 1];
						const uchar p5 = next[column + 1];
						const uchar p6 = next[column];
						const uchar p7 = next[column - 1];
						const uchar p8 = current[column - 1];
						const uchar p9 = previous[column - 1];

						const int c = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) +
							(p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) +
							(p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
							(p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);

						const int n1 = (p9 | p2) + (p3 | p4) + (p5 | p6) + (p7 | p8);
						const int n2 = (p2 | p3) + (p4 | p5) + (p6 | p7) + (p8 | p9);
						const int n = std::min(n1, n2);

						const int m = (p2 | p3 | !p5) & p4;

						if (c == 1 && (n >= 2 && n <= 3) && m == 0)
						{
							marker_pointer[column] = 1;
						}
					}
				}
			});

			binary_image &= ~marker;
		}
	};
}
