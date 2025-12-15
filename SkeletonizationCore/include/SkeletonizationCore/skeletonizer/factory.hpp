#pragma once

#include <memory>

#include "skeletonizer.hpp"

namespace skeletonizer
{
	class skeletonizer_factory
	{
	public:
		virtual ~skeletonizer_factory() = default;

		virtual std::unique_ptr<skeletonizer<>> create_skeletonizer() = 0;

		virtual std::unique_ptr<skeletonizer<>> create_skeletonizer(
			skeletonizer_type skeletonizer_type) = 0;
	};
}
