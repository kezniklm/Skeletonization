module;

#include <array>
#include <cstdint>
#include <vector>
#include <opencv2/core.hpp>

export module skeletonizer_cpu:k3m;

import skeletonizer;
import image_processing;

namespace skeletonizer::cpu::algorithms
{
	template <int... Values>
	consteval std::array<uint8_t, 256> build_lut()
	{
		std::array<uint8_t, 256> lut{};
		((lut[static_cast<size_t>(Values)] = 1), ...);
		return lut;
	}

	export class k3m final : public backend_cpu, public ::skeletonizer::algorithms::k3m
	{
	public:
		static constexpr auto A0 = build_lut<
			3, 6, 7, 12, 14, 15, 24, 28, 30, 31, 48, 56, 60, 62, 63, 96, 112, 120, 124, 126, 127,
			129, 131, 135, 143, 159, 191, 192, 193, 195, 199, 207, 223, 224, 225, 227, 231,
			239, 240, 241, 243, 247, 248, 249, 251, 252, 253, 254>();

		static constexpr auto A1 = build_lut<7, 14, 28, 56, 112, 131, 193, 224>();
		static constexpr auto A2 = build_lut<
			7, 14, 15, 28, 30, 56, 60, 112, 120, 131, 135, 193, 195, 224, 225, 240>();
		static constexpr auto A3 = build_lut<
			7, 14, 15, 28, 30, 31, 56, 60, 62, 112, 120, 124, 131, 135, 143, 193, 195, 199,
			224, 225, 227, 240, 241, 248>();
		static constexpr auto A4 = build_lut<
			7, 14, 15, 28, 30, 31, 56, 60, 62, 63, 112, 120, 124, 126, 131, 135, 143, 159, 193,
			195, 199, 207, 224, 225, 227, 231, 240, 241, 243, 248, 249, 252>();
		static constexpr auto A5 = build_lut<
			7, 14, 15, 28, 30, 31, 56, 60, 62, 63, 112, 120, 124, 126, 131, 135, 143, 159, 191,
			193, 195, 199, 207, 224, 225, 227, 231, 239, 240, 241, 243, 248, 249, 251, 252, 254>();
		static constexpr auto A1pix = build_lut<
			3, 6, 7, 12, 14, 15, 24, 28, 30, 31, 48, 56, 60, 62, 63, 96, 112, 120, 124, 126, 127,
			129, 131, 135, 143, 159, 191, 192, 193, 195, 199, 207, 223, 224, 225, 227, 231,
			239, 240, 241, 243, 247, 248, 249, 251, 252, 253, 254>();

		void apply(cv::Mat& binary_image) const override
		{
			clear_border(binary_image);

			std::vector<candidate> candidates;
			candidates.reserve(binary_image.rows * binary_image.cols / 2);

			bool any_deleted;
			do
			{
				phase_0(binary_image, candidates);

				any_deleted = delete_phase(binary_image, candidates, A1);
				any_deleted |= delete_phase(binary_image, candidates, A2);
				any_deleted |= delete_phase(binary_image, candidates, A3);
				any_deleted |= delete_phase(binary_image, candidates, A4);
				any_deleted |= delete_phase(binary_image, candidates, A5);
				// Phase 6 (unmark) implicit: candidates rebuilt each outer iteration
			}
			while (any_deleted);

			thin_to_one_pixel_width(binary_image);
		}

	private:
		struct candidate
		{
			int row;
			int column;
		};

		static inline std::uint8_t calculate_neighbour_weight(const std::uint8_t* upper_row,
		                                                      const std::uint8_t* current_row,
		                                                      const std::uint8_t* lower_row,
		                                                      const int column) noexcept
		{
			std::uint8_t mask = 0;
			mask |= upper_row[column] != 0 ? 1 : 0;
			mask |= upper_row[column + 1] != 0 ? 2 : 0;
			mask |= current_row[column + 1] != 0 ? 4 : 0;
			mask |= lower_row[column + 1] != 0 ? 8 : 0;
			mask |= lower_row[column] != 0 ? 16 : 0;
			mask |= lower_row[column - 1] != 0 ? 32 : 0;
			mask |= current_row[column - 1] != 0 ? 64 : 0;
			mask |= upper_row[column - 1] != 0 ? 128 : 0;
			return mask;
		}

		static inline void phase_0(const cv::Mat& binary_image, std::vector<candidate>& candidates)
		{
			candidates.clear();

			for (auto row = 1; row < binary_image.rows - 1; ++row)
			{
				const auto* upper_row = binary_image.ptr<std::uint8_t>(row - 1);
				const auto* current_row = binary_image.ptr<std::uint8_t>(row);
				const auto* lower_row = binary_image.ptr<std::uint8_t>(row + 1);

				for (auto col = 1; col < binary_image.cols - 1; ++col)
				{
					if (current_row[col] == background)
					{
						continue;
					}

					if (A0[calculate_neighbour_weight(upper_row, current_row, lower_row, col)])
					{
						candidates.push_back({row, col});
					}
				}
			}
		}

		static inline bool delete_phase(cv::Mat& binary_image,
		                                const std::vector<candidate>& candidates,
		                                const std::array<std::uint8_t, 256>& Ai)
		{
			bool deleted = false;

			int current_row_number = std::numeric_limits<int>::min();

			const std::uint8_t* upper_row = nullptr;
			std::uint8_t* current_row = nullptr;
			const std::uint8_t* lower_row = nullptr;

			for (const auto& [row, column] : candidates)
			{
				if (row != current_row_number)
				{
					current_row_number = row;
					upper_row = binary_image.ptr<std::uint8_t>(current_row_number - 1);
					current_row = binary_image.ptr<std::uint8_t>(current_row_number);
					lower_row = binary_image.ptr<std::uint8_t>(current_row_number + 1);
				}

				if (current_row[column] == background)
				{
					continue;
				}

				const auto neighbour_weight = calculate_neighbour_weight(upper_row, current_row, lower_row, column);

				if (!Ai[neighbour_weight])
				{
					continue;
				}

				current_row[column] = background;
				deleted = true;
			}

			return deleted;
		}

		static inline void thin_to_one_pixel_width(cv::Mat& binary_image)
		{
			bool changed;

			do
			{
				changed = false;
				for (auto row = 1; row < binary_image.rows - 1; ++row)
				{
					const auto upper_row = binary_image.ptr<std::uint8_t>(row - 1);
					const auto current_row = binary_image.ptr<std::uint8_t>(row);
					const auto lower_row = binary_image.ptr<std::uint8_t>(row + 1);

					for (auto column = 1; column < binary_image.cols - 1; ++column)
					{
						if (current_row[column] == background)
						{
							continue;
						}

						if (A1pix[calculate_neighbour_weight(upper_row, current_row, lower_row, column)])
						{
							current_row[column] = background;
							changed = true;
						}
					}
				}
			}
			while (changed);
		}
	};
}
