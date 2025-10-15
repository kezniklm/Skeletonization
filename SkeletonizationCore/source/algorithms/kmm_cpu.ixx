module;

#include <algorithm>
#include <array>
#include <cstdint>
#include <limits>

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"

export module skeletonizer_cpu:kmm;

import :core;

export namespace skeletonizer::cpu::algorithms
{
	class kmm_cpu final : public skeletonizer_cpu, public ::skeletonizer::algorithms::kmm
	{
	public:
		void apply(cv::Mat& binary_image) const override
		{
			auto workspace = create_workspace_matrix(binary_image);

			region_of_interest roi{1, workspace.rows - 1, 1, workspace.cols - 1};

			auto any_deleted = true;

			auto guard = std::max(200, (workspace.rows - 2) * (workspace.cols - 2));

			while (any_deleted && guard-- > 0)
			{
				any_deleted = false;

				stage2_label(workspace, roi);

				stage3_mark_stick(workspace, roi);

				region_of_interest touched;

				any_deleted |= stage4_delete_marked(workspace, roi, touched);
				any_deleted |= stage5_delete_array(workspace, edge, roi, touched);
				any_deleted |= stage5_delete_array(workspace, corner, roi, touched);

				if (touched.empty())
				{
					continue;
				}

				collapse_to_binary(workspace, touched.expanded(1, workspace.rows, workspace.cols));

				roi = touched.expanded(halo_next_roi, workspace.rows, workspace.cols);
			}

			workspace(cv::Rect(1, 1, binary_image.cols, binary_image.rows)).copyTo(binary_image);
		}

	private:
		static constexpr auto edge = 2;
		static constexpr auto corner = 3;
		static constexpr auto mark = 4;

		static constexpr auto halo_touch = 1;
		static constexpr auto halo_collapse = 1;
		static constexpr auto halo_next_roi = 2;

		static cv::Mat create_workspace_matrix(const cv::Mat& binary_image)
		{
			cv::Mat workspace;

			cv::copyMakeBorder(binary_image, workspace, 1, 1, 1, 1, cv::BORDER_CONSTANT, cv::Scalar(0));

			return workspace;
		}

		struct region_of_interest
		{
			int y0{0}, y1{0};
			int x0{0}, x1{0};

			bool empty() const { return y0 >= y1 || x0 >= x1; }

			region_of_interest expanded(const int halo, const int h, const int w) const
			{
				if (empty())
				{
					return *this;
				}

				region_of_interest region_of_interest;

				region_of_interest.y0 = std::max(1, y0 - halo);
				region_of_interest.y1 = std::min(h - 1, y1 + halo);
				region_of_interest.x0 = std::max(1, x0 - halo);
				region_of_interest.x1 = std::min(w - 1, x1 + halo);

				return region_of_interest;
			}

			void widen_to_include_row_span(const int y, const int x_begin, const int x_end,
			                               const region_of_interest& clamp_to)
			{
				if (x_begin > x_end)
				{
					return;
				}

				if (empty())
				{
					y0 = y;
					y1 = y + 1;
					x0 = std::clamp(x_begin, clamp_to.x0, clamp_to.x1);
					x1 = std::clamp(x_end + 1, clamp_to.x0, clamp_to.x1);

					return;
				}

				y0 = std::min(y0, y);
				y1 = std::max(y1, y + 1);
				x0 = std::min(x0, std::max(clamp_to.x0, x_begin));
				x1 = std::max(x1, std::min(clamp_to.x1, x_end + 1));
			}
		};

		static void collapse_to_binary(const cv::Mat& matrix, const region_of_interest& region_of_interest)
		{
			if (region_of_interest.y0 >= region_of_interest.y1 || region_of_interest.x0 >= region_of_interest.x1)
			{
				return;
			}

			const cv::Rect rc(region_of_interest.x0, region_of_interest.y0,
			                  region_of_interest.x1 - region_of_interest.x0,
			                  region_of_interest.y1 - region_of_interest.y0);

			cv::Mat roi = matrix(rc);

			cv::threshold(roi, roi, background, foreground, cv::THRESH_BINARY);
		}

		static inline uint8_t neighbour_mask(const uint8_t* previous, const uint8_t* current, const uint8_t* next,
		                                     const int x)
		{
			return (previous[x - 1] != 0) << 7 |
				(previous[x + 0] != 0) << 0 |
				(previous[x + 1] != 0) << 1 |
				(current[x - 1] != 0) << 6 |
				(current[x + 1] != 0) << 2 |
				(next[x - 1] != 0) << 5 |
				(next[x + 0] != 0) << 4 |
				(next[x + 1] != 0) << 3;
		}

		static void stage2_label(cv::Mat& matrix, const region_of_interest& roi)
		{
			for (auto y = roi.y0; y < roi.y1; ++y)
			{
				const auto previous = matrix.ptr<uint8_t>(y - 1);

				const auto current = matrix.ptr<uint8_t>(y + 0);

				const auto next = matrix.ptr<uint8_t>(y + 1);

				for (auto x = roi.x0; x < roi.x1; ++x)
				{
					if (current[x] != foreground)
					{
						continue;
					}

					const bool all4 = previous[x] && current[x - 1] && current[x + 1] && next[x];

					if (!all4)
					{
						current[x] = edge;

						continue;
					}

					if (!(previous[x - 1] && previous[x + 1] && next[x - 1] && next[x + 1]))
					{
						current[x] = corner;
					}
				}
			}
		}

		static void stage3_mark_stick(cv::Mat& matrix, const region_of_interest& roi)
		{
			for (auto y = roi.y0; y < roi.y1; ++y)
			{
				const auto previous = matrix.ptr<uint8_t>(y - 1);

				const auto current = matrix.ptr<uint8_t>(y + 0);

				const auto next = matrix.ptr<uint8_t>(y + 1);

				for (auto x = roi.x0; x < roi.x1; ++x)
				{
					const auto v = current[x];

					if (v != edge && v != corner)
					{
						continue;
					}

					const auto s = neighbour_mask(previous, current, next, x);

					if (stick_lut[s])
					{
						current[x] = mark;
					}
				}
			}
		}

		static bool stage4_delete_marked(cv::Mat& matrix, const region_of_interest& roi,
		                                 region_of_interest& touched_out)
		{
			bool any = false;

			for (auto y = roi.y0; y < roi.y1; ++y)
			{
				const auto current = matrix.ptr<uint8_t>(y);

				auto x_first = roi.x1;
				auto x_last = roi.x0 - 1;

				for (auto x = roi.x0; x < roi.x1; ++x)
				{
					if (current[x] != mark)
					{
						continue;
					}

					current[x] = background;
					any = true;
					x_first = std::min(x_first, x);
					x_last = std::max(x_last, x);
				}

				if (x_first <= x_last)
				{
					touched_out.widen_to_include_row_span(y, x_first - halo_touch, x_last + halo_touch, roi);
				}
			}
			return any;
		}

		static bool stage5_delete_array(cv::Mat& m, const uint8_t target, const region_of_interest& roi,
		                                region_of_interest& touched_out)
		{
			bool any = false;

			for (auto y = roi.y0; y < roi.y1; ++y)
			{
				const auto previous = m.ptr<uint8_t>(y - 1);
				const auto current = m.ptr<uint8_t>(y + 0);
				const auto next = m.ptr<uint8_t>(y + 1);

				auto x_first = roi.x1;
				auto x_last = roi.x0 - 1;

				for (auto x = roi.x0; x < roi.x1; ++x)
				{
					if (current[x] != target)
					{
						continue;
					}

					const auto s = neighbour_mask(previous, current, next, x);

					if (!delete_lut[s])
					{
						current[x] = foreground;
					}
					else
					{
						current[x] = background;
						any = true;
						x_first = std::min(x_first, x);
						x_last = std::max(x_last, x);
					}
				}

				if (x_first <= x_last)
				{
					touched_out.widen_to_include_row_span(y, x_first - halo_touch, x_last + halo_touch, roi);
				}
			}

			return any;
		}

		static constexpr uint8_t stick_values[] = {
			3, 6, 12, 24, 48, 96, 192, 129,
			7, 14, 28, 56, 112, 224, 193, 131,
			15, 30, 60, 120, 240, 225, 195, 135
		};

		static constexpr std::array<uint8_t, 256> make_stick_lut()
		{
			std::array<uint8_t, 256> array{};

			for (const auto v : stick_values)
			{
				array[v] = 1;
			}

			return array;
		}

		static inline const std::array<uint8_t, 256> stick_lut = make_stick_lut();

		static constexpr uint8_t deletion_array[] = {
			3, 5, 7, 12, 13, 14, 15, 20, 21, 22, 23, 28, 29, 30, 31, 48,
			52, 53, 54, 55, 56, 60, 61, 62, 63, 65, 67, 69, 71, 77, 79, 80,
			81, 83, 84, 85, 86, 87, 88, 89, 91, 92, 93, 94, 95, 97, 99, 101,
			103, 109, 111, 112, 113, 115, 116, 117, 118, 119, 120, 121, 123, 124, 125, 126,
			127, 131, 133, 135, 141, 143, 149, 151, 157, 159, 181, 183, 189, 191, 192, 193,
			195, 197, 199, 205, 207, 208, 209, 211, 212, 213, 214, 215, 216, 217, 219, 220,
			221, 222, 223, 224, 225, 227, 229, 231, 237, 239, 240, 241, 243, 244, 245, 246,
			247, 248, 249, 251, 252, 253, 254, 255
		};

		static constexpr std::array<uint8_t, 256> make_delete_lut()
		{
			std::array<uint8_t, 256> array{};

			for (const auto v : deletion_array)
			{
				array[v] = 1;
			}

			return array;
		}

		static inline const std::array<uint8_t, 256> delete_lut = make_delete_lut();
	};
}
