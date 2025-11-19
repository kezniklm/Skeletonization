module;

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"

export module skeletonizer:algorithms;

import :core;

namespace skeletonizer::algorithms
{
	export class zhang_suen : virtual public skeletonizer<>
	{
		std::string name() const final
		{
			return "Zhang-Suen";
		}
	};

	export class guo_hall : virtual public skeletonizer<>
	{
	public:
		std::string name() const final
		{
			return "Guo-Hall";
		}
	};

	export class kwon_gi_kang : virtual public skeletonizer<>
	{
		std::string name() const final
		{
			return "Kwon-Gi-Kang";
		}
	};

	export class petrosino_salvi : virtual public skeletonizer<>
	{
		std::string name() const final
		{
			return "Petrosino-Salvi";
		}
	};

	export class han_la_rhee : virtual public skeletonizer<>
	{
		std::string name() const final
		{
			return "Han-La-Rhee";
		}

	protected:
		template <typename... Args>
		static inline bool any_greater_or_equal_than(const uchar threshold, Args... values)
		{
			const std::array<uchar, sizeof...(values)> array = {static_cast<uchar>(values)...};

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
		static inline bool all_non_zero(Args... values)
		{
			const std::array<uchar, sizeof...(values)> array = {static_cast<uchar>(values)...};

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
		static inline bool any_equal_to(const uchar value, Args... values)
		{
			const std::array<uchar, sizeof...(values)> array = {static_cast<uchar>(values)...};

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
		static inline bool has_adjacent_non_zero(Args... values)
		{
			const std::array<uchar, sizeof...(values)> arr = {static_cast<uchar>(values)...};

			const size_t n = sizeof(arr) / sizeof(arr[0]);

			for (size_t i = 0; i < n; ++i)
			{
				if (arr[i] == 0 || arr[(i + 1) % n] == 0)
				{
					continue;
				}

				return true;
			}

			return false;
		}

		static inline bool has_critical_pairs(
			const uchar x1, const uchar x2, const uchar x3, const uchar x4,
			const uchar x5, const uchar x6, const uchar x7, const uchar x8)
		{
			return has_adjacent_non_zero(x1, x2, x3, x4, x5, x6, x7, x8) ||
				(x2 && x4) || (x4 && x6) ||
				(x6 && x8) || (x8 && x2);
		}
	};

	export class choi_lam_siu : virtual public skeletonizer<>
	{
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
				: nearest_background_row_index(std::move(row)),
				  nearest_background_column_index(std::move(col))
			{
			}

			[[nodiscard]] inline bool is_valid() const noexcept
			{
				return !nearest_background_row_index.empty() && !nearest_background_column_index.empty();
			}
		};

		static inline cv::Mat compute_nearest_background_labels(const cv::Mat& binary_image)
		{
			cv::Mat distances, labels;

			cv::distanceTransform(binary_image, distances, labels, cv::DIST_L2, cv::DIST_MASK_3, cv::DIST_LABEL_PIXEL);

			return labels;
		}

		[[nodiscard]] static inline int get_max_array_value(const cv::Mat& labels) noexcept
		{
			double min_value, max_value;

			cv::minMaxLoc(labels, &min_value, &max_value);

			return static_cast<int>(max_value);
		}

		template <typename... CommonType>
		static constexpr std::common_type_t<CommonType...> squared_length(CommonType... value) noexcept
		{
			return ((value * value) + ...);
		}
	};

	export class kmm : virtual public skeletonizer<>
	{
		std::string name() const final
		{
			return "KMM";
		}
	};

	export class k3m : virtual public skeletonizer<>
	{
		std::string name() const final
		{
			return "K3M";
		}
	};

	export class tarabek : virtual public skeletonizer<>
	{
		std::string name() const final
		{
			return "Tarabek";
		}

	protected:
		using mask3 = std::array<int8_t, 9>;

		struct mask_bits
		{
			uint16_t fixed_mask;
			uint16_t fixed_value;
			uint16_t n4_mask;
			uint16_t nd_mask;
			uint8_t n4_marked_count;
		};

		static constexpr std::array<mask_bits, 32> templates_a_bits = {
			{
				mask_bits{510, 254, 170, 68, 4}, mask_bits{507, 443, 170, 257, 4},
				mask_bits{447, 443, 170, 261, 4}, mask_bits{255, 254, 170, 69, 4},
				mask_bits{510, 254, 170, 68, 4}, mask_bits{507, 443, 170, 257, 4},
				mask_bits{447, 443, 170, 261, 4}, mask_bits{255, 254, 170, 69, 4},
				mask_bits{510, 254, 170, 68, 4}, mask_bits{507, 443, 170, 257, 4},
				mask_bits{447, 443, 170, 261, 4}, mask_bits{255, 254, 170, 69, 4},
				mask_bits{510, 254, 170, 68, 4}, mask_bits{507, 443, 170, 257, 4},
				mask_bits{447, 443, 170, 261, 4}, mask_bits{255, 254, 170, 69, 4},
				mask_bits{510, 254, 170, 68, 4}, mask_bits{507, 443, 170, 257, 4},
				mask_bits{447, 443, 170, 261, 4}, mask_bits{255, 254, 170, 69, 4},
				mask_bits{510, 254, 170, 68, 4}, mask_bits{507, 443, 170, 257, 4},
				mask_bits{447, 443, 170, 261, 4}, mask_bits{255, 254, 170, 69, 4},
				mask_bits{510, 254, 170, 68, 4}, mask_bits{507, 443, 170, 257, 4},
				mask_bits{447, 443, 170, 261, 4}, mask_bits{255, 254, 170, 69, 4},
				mask_bits{510, 254, 170, 68, 4}, mask_bits{507, 443, 170, 257, 4},
				mask_bits{447, 443, 170, 261, 4}, mask_bits{255, 254, 170, 69, 4}
			}
		};

		static constexpr std::array<mask_bits, 16> templates_b_bits = {
			{
				mask_bits{511, 443, 170, 325, 4}, mask_bits{511, 254, 170, 68, 4},
				mask_bits{511, 509, 168, 325, 3}, mask_bits{511, 479, 138, 325, 3},
				mask_bits{511, 443, 170, 325, 4}, mask_bits{511, 254, 170, 68, 4},
				mask_bits{511, 509, 168, 325, 3}, mask_bits{511, 479, 138, 325, 3},
				mask_bits{511, 443, 170, 325, 4}, mask_bits{511, 254, 170, 68, 4},
				mask_bits{511, 509, 168, 325, 3}, mask_bits{511, 479, 138, 325, 3},
				mask_bits{511, 443, 170, 325, 4}, mask_bits{511, 254, 170, 68, 4},
				mask_bits{511, 509, 168, 325, 3}, mask_bits{511, 479, 138, 325, 3}
			}
		};

		static inline bool aux_conditions_met_bits(const mask_bits& mb, const uint16_t neigh_bits) noexcept
		{
			const auto n4_marked_mask = static_cast<uint16_t>(mb.fixed_mask & mb.n4_mask);

			const auto n4_marked_count = std::popcount(n4_marked_mask);

			if (n4_marked_count >= 2)
			{
				const auto n4_background_mask = static_cast<uint16_t>(n4_marked_mask & ~neigh_bits);

				const auto n4_background_count = std::popcount(n4_background_mask);

				if (n4_background_count == n4_marked_count)
				{
					return true;
				}
			}

			if (n4_marked_count >= 1)
			{
				const auto n4_background_mask = static_cast<uint16_t>(n4_marked_mask & ~neigh_bits);

				const auto n4_bg_count = std::popcount(n4_background_mask);

				if (n4_bg_count == 1)
				{
					const auto nd_marked_mask = static_cast<uint16_t>(mb.fixed_mask & mb.nd_mask);

					const auto nd_foreground_mask = static_cast<uint16_t>(nd_marked_mask & neigh_bits);

					const auto nd_foreground_count = std::popcount(nd_foreground_mask);

					if (nd_foreground_count >= 1)
					{
						return true;
					}
				}
			}

			return false;
		}

		static inline uint16_t compute_neigh_bits(const uint8_t* previous, const uint8_t* current,
		                                          const uint8_t* next, const int column) noexcept
		{
			return static_cast<uint16_t>(
				static_cast<uint16_t>(previous[column - 1] ? 1u : 0u) << 0 |
				static_cast<uint16_t>(previous[column] ? 1u : 0u) << 1 |
				static_cast<uint16_t>(previous[column + 1] ? 1u : 0u) << 2 |
				static_cast<uint16_t>(current[column - 1] ? 1u : 0u) << 3 |
				static_cast<uint16_t>(current[column] ? 1u : 0u) << 4 |
				static_cast<uint16_t>(current[column + 1] ? 1u : 0u) << 5 |
				static_cast<uint16_t>(next[column - 1] ? 1u : 0u) << 6 |
				static_cast<uint16_t>(next[column] ? 1u : 0u) << 7 |
				static_cast<uint16_t>(next[column + 1] ? 1u : 0u) << 8
			);
		}
	};

	export class liu_zhang : virtual public skeletonizer<>
	{
		std::string name() const final
		{
			return "Liu_Zhang";
		}
	};
}
