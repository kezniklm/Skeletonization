/**
*
* @file algorithms.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Defines shared algorithm base classes and helpers.
*
* This header provides common algorithm class declarations and reusable helper
* logic used by backend-specific implementations. It centralizes algorithm
* naming and shared utility predicates for thinning conditions.
*
* Main responsibilities:
* - declare common algorithm base classes
* - provide shared helper predicates for neighborhood checks
* - define reusable mask metadata for template-driven rules
*
* @version 1.0
* @date 2026-03-16
*/

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
	/**
	 * @class zhang_suen
	 * @brief Represents shared metadata for the zhang-suen algorithm.
	 *
	 * This class provides algorithm naming used by backend-specific zhang-suen
	 * implementations.
	 */
	class zhang_suen : public virtual skeletonizer<>
	{
	public:
		/**
		 * @brief Returns the algorithm name.
		 *
		 * @return Algorithm display name.
		 */
		std::string name() const final
		{
			return "Zhang-Suen";
		}
	};

	/**
	 * @class guo_hall
	 * @brief Represents shared metadata for the guo-hall algorithm.
	 *
	 * This class provides algorithm naming used by backend-specific guo-hall
	 * implementations.
	 */
	class guo_hall : public virtual skeletonizer<>
	{
	public:
		/**
		 * @brief Returns the algorithm name.
		 *
		 * @return Algorithm display name.
		 */
		std::string name() const final
		{
			return "Guo-Hall";
		}
	};

	/**
	 * @class kwon_kang
	 * @brief Represents shared metadata for the kwon-gi-kang algorithm.
	 *
	 * This class provides algorithm naming used by backend-specific kwon-gi-kang
	 * implementations.
	 */
	class kwon_kang : public virtual skeletonizer<>
	{
	public:
		/**
		 * @brief Returns the algorithm name.
		 *
		 * @return Algorithm display name.
		 */
		std::string name() const final
		{
			return "Kwon-Gi-Kang";
		}
	};

	/**
	 * @class petrosino_salvi
	 * @brief Represents shared metadata for the petrosino-salvi algorithm.
	 *
	 * This class provides algorithm naming used by backend-specific
	 * petrosino-salvi implementations.
	 */
	class petrosino_salvi : public virtual skeletonizer<>
	{
	public:
		/**
		 * @brief Returns the algorithm name.
		 *
		 * @return Algorithm display name.
		 */
		std::string name() const final
		{
			return "Petrosino-Salvi";
		}
	};

	/**
	 * @class han_la_rhee
	 * @brief Represents shared logic for the han-la-rhee algorithm.
	 *
	 * This class exposes reusable neighborhood predicates used by backend
	 * implementations of the han-la-rhee thinning procedure.
	 */
	class han_la_rhee : public virtual skeletonizer<>
	{
	public:
		/**
		 * @brief Returns the algorithm name.
		 *
		 * @return Algorithm display name.
		 */
		std::string name() const final
		{
			return "Han-La-Rhee";
		}

	protected:
		/**
		 * @brief Checks whether any value is greater than or equal to threshold.
		 *
		 * @param threshold Threshold value.
		 * @param values Values to evaluate.
		 * @return True when at least one value satisfies the threshold.
		 */
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

		/**
		 * @brief Checks whether all values are non-zero.
		 *
		 * @param values Values to evaluate.
		 * @return True when all values are non-zero.
		 */
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

		/**
		 * @brief Checks whether any value equals a given value.
		 *
		 * @param value Target value.
		 * @param values Values to evaluate.
		 * @return True when at least one value equals target.
		 */
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

		/**
		 * @brief Checks whether two adjacent values are non-zero.
		 *
		 * @param values Circularly ordered neighborhood values.
		 * @return True when any adjacent pair is non-zero.
		 */
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

		/**
		 * @brief Checks critical neighborhood pair conditions.
		 *
		 * @param x1 Neighbor value x1.
		 * @param x2 Neighbor value x2.
		 * @param x3 Neighbor value x3.
		 * @param x4 Neighbor value x4.
		 * @param x5 Neighbor value x5.
		 * @param x6 Neighbor value x6.
		 * @param x7 Neighbor value x7.
		 * @param x8 Neighbor value x8.
		 * @return True when critical pair rule is satisfied.
		 */
		static bool has_critical_pairs(
			uchar x1, uchar x2, uchar x3, uchar x4,
			uchar x5, uchar x6, uchar x7, uchar x8)
		{
			return has_adjacent_non_zero(x1, x2, x3, x4, x5, x6, x7, x8) ||
				(x2 && x4) || (x4 && x6) ||
				(x6 && x8) || (x8 && x2);
		}
	};

	/**
	 * @class choi_lam_siu
	 * @brief Represents shared logic for the choi-lam-siu algorithm.
	 *
	 * This class provides distance-map helpers and geometric predicates used by
	 * backend implementations of choi-lam-siu thinning.
	 */
	class choi_lam_siu : public virtual skeletonizer<>
	{
	public:
		/**
		 * @brief Returns the algorithm name.
		 *
		 * @return Algorithm display name.
		 */
		std::string name() const final
		{
			return "Choi-Lam-Siu";
		}

	protected:
		/**
		 * @class xy_distance_maps
		 * @brief Stores nearest-background indices for rows and columns.
		 *
		 * This structure keeps precomputed label maps used by choi-lam-siu
		 * distance conditions.
		 */
		struct xy_distance_maps
		{
			/// Row index map to nearest background pixels.
			const cv::Mat nearest_background_row_index;
			/// Column index map to nearest background pixels.
			const cv::Mat nearest_background_column_index;

			/**
			 * @brief Constructs a new xy_distance_maps object.
			 *
			 * @param row Row index map.
			 * @param col Column index map.
			 */
			explicit xy_distance_maps(cv::Mat row, cv::Mat col)
				: nearest_background_row_index(std::move(row))
				  , nearest_background_column_index(std::move(col))
			{
			}

			/**
			 * @brief Checks whether both maps are initialized.
			 *
			 * @return True when both maps are non-empty.
			 */
			[[nodiscard]] bool is_valid() const noexcept
			{
				return !nearest_background_row_index.empty() &&
					!nearest_background_column_index.empty();
			}
		};

		/**
		 * @brief Computes nearest-background label matrix.
		 *
		 * @param binary_image Input binary image.
		 * @return Label matrix from distance transform.
		 */
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

		/**
		 * @brief Returns the maximum value contained in a matrix.
		 *
		 * @param labels Input matrix.
		 * @return Maximum value converted to integer.
		 */
		[[nodiscard]] static int get_max_array_value(const cv::Mat& labels) noexcept
		{
			double min_value = 0.0;
			double max_value = 0.0;
			cv::minMaxLoc(labels, &min_value, &max_value);
			return static_cast<int>(max_value);
		}

		/**
		 * @brief Computes sum of squared values.
		 *
		 * @param value Values to square and sum.
		 * @return Squared length.
		 */
		template <typename... CommonType>
		static constexpr std::common_type_t<CommonType...>
		squared_length(CommonType... value) noexcept
		{
			return ((value * value) + ...);
		}
	};

	/**
	 * @class kmm
	 * @brief Represents shared metadata for the kmm algorithm.
	 *
	 * This class provides algorithm naming for backend-specific kmm
	 * implementations.
	 */
	class kmm : public virtual skeletonizer<>
	{
	public:
		/**
		 * @brief Returns the algorithm name.
		 *
		 * @return Algorithm display name.
		 */
		std::string name() const final
		{
			return "KMM";
		}
	};

	/**
	 * @class k3m
	 * @brief Represents shared metadata for the k3m algorithm.
	 *
	 * This class provides algorithm naming for backend-specific k3m
	 * implementations.
	 */
	class k3m : public virtual skeletonizer<>
	{
	public:
		/**
		 * @brief Returns the algorithm name.
		 *
		 * @return Algorithm display name.
		 */
		std::string name() const final
		{
			return "K3M";
		}
	};

	/**
	 * @class tarabek
	 * @brief Represents shared masks and helpers for the tarabek algorithm.
	 *
	 * This class defines precomputed mask templates and bitwise neighborhood
	 * helper functions reused by tarabek implementations.
	 */
	class tarabek : public virtual skeletonizer<>
	{
	public:
		/**
		 * @brief Returns the algorithm name.
		 *
		 * @return Algorithm display name.
		 */
		std::string name() const final
		{
			return "Tarabek";
		}

	protected:
		/// 3x3 mask representation.
		using mask3 = std::array<std::int8_t, 9>;

		/**
		 * @class mask_bits
		 * @brief Stores precomputed bit masks for template matching.
		 *
		 * This structure encodes fixed and conditional masks used by tarabek
		 * neighborhood tests.
		 */
		struct mask_bits
		{
			/// Bit mask of fixed positions.
			std::uint16_t fixed_mask;
			/// Required bit values for fixed positions.
			std::uint16_t fixed_value;
			/// Cardinal-neighbor mask.
			std::uint16_t n4_mask;
			/// Diagonal-neighbor mask.
			std::uint16_t nd_mask;
			/// Count of marked cardinal neighbors.
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

		/**
		 * @brief Evaluates auxiliary template conditions for neighborhood bits.
		 *
		 * @param mb Template mask metadata.
		 * @param neigh_bits Encoded neighborhood bits.
		 * @return True when auxiliary conditions are satisfied.
		 */
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

		/**
		 * @brief Encodes a 3x3 neighborhood into a bit mask.
		 *
		 * @param previous Pointer to previous row data.
		 * @param current Pointer to current row data.
		 * @param next Pointer to next row data.
		 * @param column Center column index.
		 * @return Encoded neighborhood bit mask.
		 */
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

	/**
	 * @class liu_zhang
	 * @brief Represents shared metadata for the liu-zhang algorithm.
	 *
	 * This class provides algorithm naming for backend-specific liu-zhang
	 * implementations.
	 */
	class liu_zhang : public virtual skeletonizer<>
	{
	public:
		/**
		 * @brief Returns the algorithm name.
		 *
		 * @return Algorithm display name.
		 */
		std::string name() const final
		{
			return "Liu-Zhang";
		}
	};

	/**
	 * @class lantuejoul
	 * @brief Represents shared metadata for the lantuejoul algorithm.
	 *
	 * This class provides algorithm naming for backend-specific lantuejoul
	 * implementations.
	 */
	class lantuejoul : public virtual skeletonizer<>
	{
	public:
		/**
		 * @brief Returns the algorithm name.
		 *
		 * @return Algorithm display name.
		 */
		std::string name() const final
		{
			return "Lantuejoul";
		}
	};
}
