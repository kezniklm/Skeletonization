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
}
