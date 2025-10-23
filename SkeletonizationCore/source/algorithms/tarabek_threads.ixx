module;

#include <algorithm>
#include <array>
#include <cstdint>
#include <opencv2/core.hpp>

export module skeletonizer_cpu:tarabek_threads;

import :core;

export namespace skeletonizer::cpu::algorithms
{
	class tarabek_threads final : public skeletonizer_cpu, public ::skeletonizer::algorithms::tarabek
	{
	public:
		void apply(cv::Mat& binary_image) const override
		{
			cv::Mat prev(binary_image.size(), CV_8UC1, cv::Scalar(0));

			cv::Mat marker(binary_image.size(), CV_8UC1);

			cv::Mat difference;

			do
			{
				first_iteration(binary_image, marker);

				second_iteration(binary_image, marker);

				cv::absdiff(binary_image, prev, difference);

				binary_image.copyTo(prev);
			}
			while (has_changed(difference));

			postprocessing(binary_image);

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
					const auto previous = binary_image.ptr<uint8_t>(row - 1);
					const auto current = binary_image.ptr<uint8_t>(row);
					const auto next = binary_image.ptr<uint8_t>(row + 1);
					const auto marker_pointer = marker.ptr<uint8_t>(row);

					for (auto column = 1; column < binary_image.cols - 1; ++column)
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

						const int a = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) +
							(p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) +
							(p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
							(p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);

						const int b = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;

						const int step_condition_c = p2 * p4 * p6;
						const int step_condition_d = p4 * p6 * p8;

						if (a != 1 || b < 2 || b > 6 || step_condition_c != 0 || step_condition_d != 0)
						{
							continue;
						}

						if (b == 2)
						{
							const auto bn4 = p2 + p4 + p6 + p8;

							const auto an4 = (p2 == 0 && p4 == 1) +
								(p4 == 0 && p6 == 1) +
								(p6 == 0 && p8 == 1) +
								(p8 == 0 && p2 == 1);

							const auto bnd = p3 + p5 + p7 + p9;

							const auto AND = (p3 == 0 && p5 == 1) +
								(p5 == 0 && p7 == 1) +
								(p7 == 0 && p9 == 1) +
								(p9 == 0 && p3 == 1);

							if (bn4 == 3 && an4 == 2 && bnd == 4 && AND == 2)
							{
								continue;
							}
						}

						if (b == 3 && a == 1 && p4 == foreground && p5 == foreground && p6 == foreground)
						{
							continue;
						}

						marker_pointer[column] = foreground;
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
					const auto previous = binary_image.ptr<uint8_t>(row - 1);
					const auto current = binary_image.ptr<uint8_t>(row);
					const auto next = binary_image.ptr<uint8_t>(row + 1);
					const auto marker_pointer = marker.ptr<uint8_t>(row);

					for (auto column = 1; column < binary_image.cols - 1; ++column)
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

						const auto a = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) +
							(p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) +
							(p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
							(p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);

						const auto b = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;

						const auto step_condition_c = p2 * p4 * p8;

						const auto step_condition_d = p2 * p6 * p8;

						if (a != 1 || b < 2 || b > 6 || step_condition_c != 0 || step_condition_d != 0)
						{
							continue;
						}

						if (b == 2)
						{
							const auto bn4 = p2 + p4 + p6 + p8;

							const auto an4 = (p2 == 0 && p4 == 1) +
								(p4 == 0 && p6 == 1) +
								(p6 == 0 && p8 == 1) +
								(p8 == 0 && p2 == 1);

							const auto bnd = p3 + p5 + p7 + p9;

							const auto AND = (p3 == 0 && p5 == 1) +
								(p5 == 0 && p7 == 1) +
								(p7 == 0 && p9 == 1) +
								(p9 == 0 && p3 == 1);

							if (bn4 == 3 && an4 == 2 && bnd == 4 && AND == 2)
							{
								continue;
							}
						}

						if (b == 3 && a == 1 && p4 == foreground && p5 == foreground && p6 == foreground)
						{
							continue;
						}

						marker_pointer[column] = foreground;
					}
				}
			});

			binary_image &= ~marker;
		}

		template <size_t N>
		static inline void process_template_set(const cv::Mat& binary_image,
		                                        const std::array<mask_bits, N>& templates_bits,
		                                        const uint8_t background)
		{
			cv::Mat marker(binary_image.size(), CV_8UC1);

			bool changed;

			do
			{
				changed = false;

				marker.setTo(0);

				cv::parallel_for_(cv::Range(1, binary_image.rows - 1), [&](const cv::Range& range)
				{
					for (auto row = range.start; row < range.end; ++row)
					{
						const auto previous = binary_image.ptr<uint8_t>(row - 1);

						const auto current = binary_image.ptr<uint8_t>(row);

						const auto next = binary_image.ptr<uint8_t>(row + 1);

						const auto marker_pointer = marker.ptr<uint8_t>(row);

						for (auto c = 1; c < binary_image.cols - 1; ++c)
						{
							if (current[c] == background)
							{
								continue;
							}

							const auto neigh_bits = compute_neigh_bits(previous, current, next, c);

							for (size_t template_bit_position = 0; template_bit_position < templates_bits.size(); ++
							     template_bit_position)
							{
								const mask_bits& mask_bits = templates_bits[template_bit_position];

								if (((neigh_bits ^ mask_bits.fixed_value) & mask_bits.fixed_mask) != 0)
								{
									continue;
								}

								if (!aux_conditions_met_bits(mask_bits, neigh_bits))
								{
									continue;
								}

								marker_pointer[c] = background;

								changed = true;

								break;
							}
						}
					}
				});

				binary_image &= ~marker;
			}
			while (changed);
		}

		static inline void postprocessing(const cv::Mat& binary_image)
		{
			process_template_set(binary_image, templates_a_bits, background);
			process_template_set(binary_image, templates_b_bits, background);
		}
	};
}
