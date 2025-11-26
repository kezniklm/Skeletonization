module;

#include <opencv2/core.hpp>

export module skeletonizer_mt:kwon_gi_kang;

import skeletonizer;

export namespace skeletonizer::mt::algorithms
{
	class kwon_gi_kang final : public backend_threads,
	                           public ::skeletonizer::algorithms::kwon_gi_kang
	{
	public:
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

			cleanup_oblique_corners(binary_image, marker);

			clear_border(binary_image);
		}

	private:
		static void first_iteration(cv::Mat& binary_image, cv::Mat& marker)
		{
			marker.setTo(0);

			cv::parallel_for_(cv::Range(1, binary_image.rows - 1), [&](const cv::Range& range)
			{
				for (auto row = range.start; row < range.end; ++row)
				{
					const uchar* previous = binary_image.ptr<uchar>(row - 1);
					const uchar* current = binary_image.ptr<uchar>(row);
					const uchar* next = binary_image.ptr<uchar>(row + 1);
					uchar* marker_pointer = marker.ptr<uchar>(row);

					for (auto column = 1; column < binary_image.cols - 1; ++column)
					{
						const auto pi = current[column];

						if (pi == background)
						{
							continue;
						}

						const auto p1 = previous[column - 1];
						const auto p2 = previous[column];
						const auto p3 = previous[column + 1];
						const auto p4 = current[column + 1];
						const auto p5 = next[column + 1];
						const auto p6 = next[column];
						const auto p7 = next[column - 1];
						const auto p8 = current[column - 1];

						const auto a = (p1 == 0 && p2 == 1) + (p2 == 0 && p3 == 1) +
							(p3 == 0 && p4 == 1) + (p4 == 0 && p5 == 1) +
							(p5 == 0 && p6 == 1) + (p6 == 0 && p7 == 1) +
							(p7 == 0 && p8 == 1) + (p8 == 0 && p1 == 1);

						const auto b = p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8;

						const auto step_condition_c = p2 * p4 * p6;
						const auto step_condition_d = p4 * p6 * p8;

						if (a == 1 && b >= 2 && b <= 6 && step_condition_c == 0 && step_condition_d == 0)
						{
							marker_pointer[column] = skeleton;
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
				for (auto row = range.start; row < range.end; ++row)
				{
					const uchar* previous = binary_image.ptr<uchar>(row - 1);
					const uchar* current = binary_image.ptr<uchar>(row);
					const uchar* next = binary_image.ptr<uchar>(row + 1);
					uchar* marker_pointer = marker.ptr<uchar>(row);

					for (auto column = 1; column < binary_image.cols - 1; ++column)
					{
						const auto pi = current[column];

						if (pi == background)
						{
							continue;
						}

						const auto p1 = previous[column - 1];
						const auto p2 = previous[column];
						const auto p3 = previous[column + 1];
						const auto p4 = current[column + 1];
						const auto p5 = next[column + 1];
						const auto p6 = next[column];
						const auto p7 = next[column - 1];
						const auto p8 = current[column - 1];

						const auto a = (p1 == 0 && p2 == 1) + (p2 == 0 && p3 == 1) +
							(p3 == 0 && p4 == 1) + (p4 == 0 && p5 == 1) +
							(p5 == 0 && p6 == 1) + (p6 == 0 && p7 == 1) +
							(p7 == 0 && p8 == 1) + (p8 == 0 && p1 == 1);

						const auto b = p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8;

						const auto step_condition_c = p2 * p4 * p8;
						const auto step_condition_d = p2 * p6 * p8;

						if (a == 1 && b >= 2 && b <= 6 && step_condition_c == 0 && step_condition_d == 0)
						{
							marker_pointer[column] = skeleton;
						}
					}
				}
			});

			binary_image &= ~marker;
		}

		static void cleanup_oblique_corners(cv::Mat& binary_image, cv::Mat& marker)
		{
			marker.setTo(0);

			cv::parallel_for_(cv::Range(1, binary_image.rows - 1), [&](const cv::Range& range)
			{
				for (auto row = range.start; row < range.end; ++row)
				{
					const uchar* prev = binary_image.ptr<uchar>(row - 1);
					const uchar* curr = binary_image.ptr<uchar>(row);
					const uchar* next = binary_image.ptr<uchar>(row + 1);
					uchar* marker_ptr = marker.ptr<uchar>(row);

					for (auto col = 1; col < binary_image.cols - 1; ++col)
					{
						const auto pi = curr[col];

						if (pi == background)
						{
							continue;
						}

						const auto p1 = prev[col - 1];
						const auto p3 = prev[col + 1];
						const auto p4 = curr[col + 1];
						const auto p5 = next[col + 1];
						const auto p6 = next[col];
						const auto p7 = next[col - 1];
						const auto p8 = curr[col - 1];

						const auto condition_1 = p3 == 0 && p1 * p6 * p8 == 1;
						const auto condition_2 = p1 == 0 && p3 * p4 * p6 == 1;
						const auto condition_3 = p3 == 0 && p5 * p6 * p8 == 1;
						const auto condition_4 = p1 == 0 && p4 * p6 * p7 == 1;

						if (condition_1 || condition_2 || condition_3 || condition_4)
						{
							marker_ptr[col] = skeleton;
						}
					}
				}
			});

			binary_image &= ~marker;
		}
	};
}
