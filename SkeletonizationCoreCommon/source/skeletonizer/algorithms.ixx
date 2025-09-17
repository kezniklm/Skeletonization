module;

#include <opencv2/core.hpp>

export module skeletonizer:algorithms;

import :core;

namespace skeletonizer::algorithms
{
	export class zhang_suen : virtual public skeletonizer
	{
		std::string name() const final
		{
			return "Zhang-Suen";
		}
	};

	export class guo_hall : virtual public skeletonizer
	{
	public:
		std::string name() const final
		{
			return "Guo-Hall";
		}
	};

	export class hesselink_roerdink : virtual public skeletonizer
	{
		std::string name() const final
		{
			return "Hesselink-Roerdink";
		}
	};

	export class kwon_gi_kang : virtual public skeletonizer
	{
		std::string name() const final
		{
			return "Kwon-Gi-Kang";
		}
	};

	export class petrosino_salvi : virtual public skeletonizer
	{
		std::string name() const final
		{
			return "Petrosino-Salvi";
		}
	};

	export class han_la_rhee : virtual public skeletonizer
	{
	public:
		std::string name() const final
		{
			return "Han-La-Rhee";
		}

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

	export class choi_lam_siu : virtual public skeletonizer
	{
		std::string name() const final
		{
			return "Choi_Lam_Siu";
		}
	};
}
