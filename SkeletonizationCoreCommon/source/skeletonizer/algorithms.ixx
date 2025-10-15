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
}
