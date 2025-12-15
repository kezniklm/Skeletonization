#include "SkeletonizationCoreCPU/liu_zhang.hpp"

namespace skeletonizer::cpu::algorithms
{
	void liu_zhang::apply(cv::Mat& binary_image) const
	{
		cv::Mat marker(binary_image.size(), CV_8UC1);

		while (true)
		{
			// Step 1: constrained passes (mark but ignore Cp constraint in the first calls)
			first_iteration(binary_image, marker, true);
			second_iteration(binary_image, marker, true);

			// Step 2: unconstrained, check if anything else can be removed
			if (!first_iteration(binary_image, marker, false))
			{
				delete_patterns_ghij(binary_image, marker);
				break;
			}

			if (!second_iteration(binary_image, marker, false))
			{
				delete_patterns_ghij(binary_image, marker);
				break;
			}
		}

		clear_border(binary_image);
	}

	bool liu_zhang::first_iteration(cv::Mat& binary_image,
	                                cv::Mat& marker,
	                                const bool use_constraint)
	{
		marker.setTo(0);
		bool deleted = false;

		for (int row = 1; row < binary_image.rows - 1; ++row)
		{
			const auto* previous = binary_image.ptr<uchar>(row - 1);
			const auto* current = binary_image.ptr<uchar>(row);
			const auto* next = binary_image.ptr<uchar>(row + 1);
			auto* marker_pointer = marker.ptr<uchar>(row);

			for (int column = 1; column < binary_image.cols - 1; ++column)
			{
				const auto p1 = current[column];

				if (p1 == background)
				{
					continue;
				}

				const auto p2 = previous[column];
				const auto p3 = previous[column + 1];
				const auto p4 = current[column + 1];
				const auto p5 = next[column + 1];
				const auto p6 = next[column];
				const auto p7 = next[column - 1];
				const auto p8 = current[column - 1];
				const auto p9 = previous[column - 1];

				const auto a =
					(p2 == 0 && p3 == 1) +
					(p3 == 0 && p4 == 1) +
					(p4 == 0 && p5 == 1) +
					(p5 == 0 && p6 == 1) +
					(p6 == 0 && p7 == 1) +
					(p7 == 0 && p8 == 1) +
					(p8 == 0 && p9 == 1) +
					(p9 == 0 && p2 == 1);

				const auto b =
					p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;

				const auto step_condition_c = p2 * p4 * p6;
				const auto step_condition_d = p4 * p6 * p8;

				bool is_cp_ok = true;

				if (use_constraint)
				{
					const auto Cp = p2 + p4 + p6 + p8;
					is_cp_ok = Cp < 3;
				}

				if (a == 1 &&
					b >= 2 && b <= 6 &&
					step_condition_c == 0 &&
					step_condition_d == 0 &&
					is_cp_ok)
				{
					marker_pointer[column] = skeleton;
					deleted = true;
				}
			}
		}

		binary_image &= ~marker;
		return deleted;
	}

	bool liu_zhang::second_iteration(cv::Mat& binary_image,
	                                 cv::Mat& marker,
	                                 const bool use_constraint)
	{
		marker.setTo(0);
		bool deleted = false;

		for (int row = 1; row < binary_image.rows - 1; ++row)
		{
			const auto* previous = binary_image.ptr<uchar>(row - 1);
			const auto* current = binary_image.ptr<uchar>(row);
			const auto* next = binary_image.ptr<uchar>(row + 1);
			auto* marker_pointer = marker.ptr<uchar>(row);

			for (int column = 1; column < binary_image.cols - 1; ++column)
			{
				const auto p1 = current[column];

				if (p1 == background)
				{
					continue;
				}

				const auto p2 = previous[column];
				const auto p3 = previous[column + 1];
				const auto p4 = current[column + 1];
				const auto p5 = next[column + 1];
				const auto p6 = next[column];
				const auto p7 = next[column - 1];
				const auto p8 = current[column - 1];
				const auto p9 = previous[column - 1];

				const auto a =
					(p2 == 0 && p3 == 1) +
					(p3 == 0 && p4 == 1) +
					(p4 == 0 && p5 == 1) +
					(p5 == 0 && p6 == 1) +
					(p6 == 0 && p7 == 1) +
					(p7 == 0 && p8 == 1) +
					(p8 == 0 && p9 == 1) +
					(p9 == 0 && p2 == 1);

				const auto b =
					p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;

				const auto step_condition_c = p2 * p4 * p8;
				const auto step_condition_d = p2 * p6 * p8;

				bool is_cp_ok = true;

				if (use_constraint)
				{
					const auto Cp = p2 + p4 + p6 + p8;
					is_cp_ok = Cp < 3;
				}

				if (a == 1 &&
					b >= 2 && b <= 6 &&
					step_condition_c == 0 &&
					step_condition_d == 0 &&
					is_cp_ok)
				{
					marker_pointer[column] = skeleton;
					deleted = true;
				}
			}
		}

		binary_image &= ~marker;
		return deleted;
	}

	void liu_zhang::delete_patterns_ghij(cv::Mat& binary_image,
	                                     cv::Mat& marker) const
	{
		marker.setTo(0);

		for (int y = 1; y < binary_image.rows - 1; ++y)
		{
			const auto* previous = binary_image.ptr<uchar>(y - 1);
			const auto* current = binary_image.ptr<uchar>(y);
			const auto* next = binary_image.ptr<uchar>(y + 1);
			auto* marker_row = marker.ptr<uchar>(y);

			for (int x = 1; x < binary_image.cols - 1; ++x)
			{
				const auto p1 = current[x];

				if (p1 == 0)
				{
					continue;
				}

				const bool p2 = previous[x] > 0;
				const bool p3 = previous[x + 1] > 0;
				const bool p4 = current[x + 1] > 0;
				const bool p5 = next[x + 1] > 0;
				const bool p6 = next[x] > 0;
				const bool p7 = next[x - 1] > 0;
				const bool p8 = current[x - 1] > 0;
				const bool p9 = previous[x - 1] > 0;

				const int Np =
					static_cast<int>(p2) + static_cast<int>(p3) +
					static_cast<int>(p4) + static_cast<int>(p5) +
					static_cast<int>(p6) + static_cast<int>(p7) +
					static_cast<int>(p8) + static_cast<int>(p9);

				if (Np != 2)
				{
					continue;
				}

				const bool step_condition_g = p6 && p8 && !p3;
				const bool step_condition_h = p4 && p6 && !p9;
				const bool step_condition_i = p2 && p4 && !p7;
				const bool step_condition_j = p2 && p8 && !p5;

				if (step_condition_g ||
					step_condition_h ||
					step_condition_i ||
					step_condition_j)
				{
					marker_row[x] = skeleton;
				}
			}
		}

		binary_image &= ~marker;
	}
}
