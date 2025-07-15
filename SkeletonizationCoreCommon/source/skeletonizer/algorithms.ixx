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
		std::string name() const final override
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
}
