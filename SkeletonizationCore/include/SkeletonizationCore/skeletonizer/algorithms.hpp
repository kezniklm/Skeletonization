#pragma once

#include <array>
#include <bit>
#include <cstdint>
#include <string>
#include <type_traits>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include "skeletonizer.hpp"

namespace skeletonizer::algorithms
{
	class zhang_suen : public virtual skeletonizer<>
	{
	public:
		std::string name() const final
		{
			return "Zhang-Suen";
		}
	};

	class guo_hall : public virtual skeletonizer<>
	{
	public:
		std::string name() const final
		{
			return "Guo-Hall";
		}
	};

	class kwon_gi_kang : public virtual skeletonizer<>
	{
	public:
		std::string name() const final
		{
			return "Kwon-Gi-Kang";
		}
	};

	class petrosino_salvi : public virtual skeletonizer<>
	{
	public:
		std::string name() const final
		{
			return "Petrosino-Salvi";
		}
	};

	class han_la_rhee : public virtual skeletonizer<>
	{
	public:
		std::string name() const final
		{
			return "Han-La-Rhee";
		}

	protected:
		template <typename... Args>
		static bool any_greater_or_equal_than(uchar threshold, Args... values)
		{
			const std::array<uchar, sizeof...(values)> array{
				static_cast<uchar>(values)...
			};

			for (const uchar value : array)
			{
				if (value < threshold)
				{
					continue;
				}
				return true;
			}
			return false;
		}

		template <typename... Args>
		static bool all_non_zero(Args... values)
		{
			const std::array<uchar, sizeof...(values)> array{
				static_cast<uchar>(values)...
			};

			for (const uchar value : array)
			{
				if (value != 0)
				{
					continue;
				}
				return false;
			}
			return true;
		}

		template <typename... Args>
		static bool any_equal_to(uchar value, Args... values)
		{
			const std::array<uchar, sizeof...(values)> array{
				static_cast<uchar>(values)...
			};

			for (const uchar val : array)
			{
				if (val != value)
				{
					continue;
				}
				return true;
			}
			return false;
		}

		template <typename... Args>
		static bool has_adjacent_non_zero(Args... values)
		{
			const std::array<uchar, sizeof...(values)> arr{
				static_cast<uchar>(values)...
			};

			const std::size_t n = arr.size();

			for (std::size_t i = 0; i < n; ++i)
			{
				if (arr[i] == 0 || arr[(i + 1) % n] == 0)
				{
					continue;
				}
				return true;
			}
			return false;
		}

		static bool has_critical_pairs(
			uchar x1, uchar x2, uchar x3, uchar x4,
			uchar x5, uchar x6, uchar x7, uchar x8)
		{
			return has_adjacent_non_zero(x1, x2, x3, x4, x5, x6, x7, x8) ||
				(x2 && x4) || (x4 && x6) ||
				(x6 && x8) || (x8 && x2);
		}
	};

	class choi_lam_siu : public virtual skeletonizer<>
	{
	public:
		std::string name() const final
		{
			return "Choi_Lam_Siu";
		}

	protected:
		struct xy_distance_maps
		{
			const cv::Mat nearest_background_row_index;
			const cv::Mat nearest_background_column_index;

			explicit xy_distance_maps(cv::Mat row, cv::Mat col)
				: nearest_background_row_index(std::move(row))
				  , nearest_background_column_index(std::move(col))
			{
			}

			[[nodiscard]] bool is_valid() const noexcept
			{
				return !nearest_background_row_index.empty() &&
					!nearest_background_column_index.empty();
			}
		};

		static cv::Mat compute_nearest_background_labels(const cv::Mat& binary_image)
		{
			cv::Mat distances, labels;
			cv::distanceTransform(
				binary_image,
				distances,
				labels,
				cv::DIST_L2,
				cv::DIST_MASK_3,
				cv::DIST_LABEL_PIXEL);
			return labels;
		}

		[[nodiscard]] static int get_max_array_value(const cv::Mat& labels) noexcept
		{
			double min_value = 0.0;
			double max_value = 0.0;
			cv::minMaxLoc(labels, &min_value, &max_value);
			return static_cast<int>(max_value);
		}

		template <typename... CommonType>
		static constexpr std::common_type_t<CommonType...>
		squared_length(CommonType... value) noexcept
		{
			return ((value * value) + ...);
		}
	};

	class kmm : public virtual skeletonizer<>
	{
	public:
		std::string name() const final
		{
			return "KMM";
		}
	};

	class k3m : public virtual skeletonizer<>
	{
	public:
		std::string name() const final
		{
			return "K3M";
		}
	};

	class tarabek : public virtual skeletonizer<>
	{
	public:
		std::string name() const final
		{
			return "Tarabek";
		}

	protected:
		using mask3 = std::array<std::int8_t, 9>;

		struct mask_bits
		{
			std::uint16_t fixed_mask;
			std::uint16_t fixed_value;
			std::uint16_t n4_mask;
			std::uint16_t nd_mask;
			std::uint8_t n4_marked_count;
		};

		static constexpr std::array<mask_bits, 32> templates_a_bits{
			{
				{510, 254, 170, 68, 4}, {507, 443, 170, 257, 4},
				{447, 443, 170, 261, 4}, {255, 254, 170, 69, 4},
				{510, 254, 170, 68, 4}, {507, 443, 170, 257, 4},
				{447, 443, 170, 261, 4}, {255, 254, 170, 69, 4},
				{510, 254, 170, 68, 4}, {507, 443, 170, 257, 4},
				{447, 443, 170, 261, 4}, {255, 254, 170, 69, 4},
				{510, 254, 170, 68, 4}, {507, 443, 170, 257, 4},
				{447, 443, 170, 261, 4}, {255, 254, 170, 69, 4},
				{510, 254, 170, 68, 4}, {507, 443, 170, 257, 4},
				{447, 443, 170, 261, 4}, {255, 254, 170, 69, 4},
				{510, 254, 170, 68, 4}, {507, 443, 170, 257, 4},
				{447, 443, 170, 261, 4}, {255, 254, 170, 69, 4},
				{510, 254, 170, 68, 4}, {507, 443, 170, 257, 4},
				{447, 443, 170, 261, 4}, {255, 254, 170, 69, 4},
				{510, 254, 170, 68, 4}, {507, 443, 170, 257, 4},
				{447, 443, 170, 261, 4}, {255, 254, 170, 69, 4},
			}
		};

		static constexpr std::array<mask_bits, 16> templates_b_bits{
			{
				{511, 443, 170, 325, 4}, {511, 254, 170, 68, 4},
				{511, 509, 168, 325, 3}, {511, 479, 138, 325, 3},
				{511, 443, 170, 325, 4}, {511, 254, 170, 68, 4},
				{511, 509, 168, 325, 3}, {511, 479, 138, 325, 3},
				{511, 443, 170, 325, 4}, {511, 254, 170, 68, 4},
				{511, 509, 168, 325, 3}, {511, 479, 138, 325, 3},
				{511, 443, 170, 325, 4}, {511, 254, 170, 68, 4},
				{511, 509, 168, 325, 3}, {511, 479, 138, 325, 3},
			}
		};

		static bool aux_conditions_met_bits(const mask_bits& mb,
		                                    const std::uint16_t neigh_bits) noexcept
		{
			const auto n4_marked_mask =
				static_cast<std::uint16_t>(mb.fixed_mask & mb.n4_mask);

			const auto n4_marked_count =
				static_cast<std::uint8_t>(std::popcount(n4_marked_mask));

			if (n4_marked_count >= 2)
			{
				const auto n4_background_mask =
					static_cast<std::uint16_t>(n4_marked_mask & ~neigh_bits);

				const auto n4_background_count =
					static_cast<std::uint8_t>(std::popcount(n4_background_mask));

				if (n4_background_count == n4_marked_count)
				{
					return true;
				}
			}

			if (n4_marked_count >= 1)
			{
				const auto n4_background_mask =
					static_cast<std::uint16_t>(n4_marked_mask & ~neigh_bits);

				const auto n4_bg_count =
					static_cast<std::uint8_t>(std::popcount(n4_background_mask));

				if (n4_bg_count == 1)
				{
					const auto nd_marked_mask =
						static_cast<std::uint16_t>(mb.fixed_mask & mb.nd_mask);

					const auto nd_foreground_mask =
						static_cast<std::uint16_t>(nd_marked_mask & neigh_bits);

					const auto nd_foreground_count =
						static_cast<std::uint8_t>(std::popcount(nd_foreground_mask));

					if (nd_foreground_count >= 1)
					{
						return true;
					}
				}
			}

			return false;
		}

		static std::uint16_t compute_neigh_bits(const std::uint8_t* previous,
		                                        const std::uint8_t* current,
		                                        const std::uint8_t* next,
		                                        int column) noexcept
		{
			return static_cast<std::uint16_t>(
				(static_cast<std::uint16_t>(previous[column - 1] ? 1u : 0u) << 0) |
				(static_cast<std::uint16_t>(previous[column] ? 1u : 0u) << 1) |
				(static_cast<std::uint16_t>(previous[column + 1] ? 1u : 0u) << 2) |
				(static_cast<std::uint16_t>(current[column - 1] ? 1u : 0u) << 3) |
				(static_cast<std::uint16_t>(current[column] ? 1u : 0u) << 4) |
				(static_cast<std::uint16_t>(current[column + 1] ? 1u : 0u) << 5) |
				(static_cast<std::uint16_t>(next[column - 1] ? 1u : 0u) << 6) |
				(static_cast<std::uint16_t>(next[column] ? 1u : 0u) << 7) |
				(static_cast<std::uint16_t>(next[column + 1] ? 1u : 0u) << 8));
		}
	};

	class liu_zhang : public virtual skeletonizer<>
	{
	public:
		std::string name() const final
		{
			return "Liu_Zhang";
		}
	};
}
