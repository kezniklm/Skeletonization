module;

#include <memory>

export module skeletonizer_factory;

import skeletonizer;

export class skeletonizer_factory
{
public:
	virtual ~skeletonizer_factory() = default;

	virtual std::unique_ptr<skeletonizer> create_skeletonizer() = 0;

	virtual std::unique_ptr<skeletonizer> create_skeletonizer(skeletonizer_type skeletonizer_type) = 0;
};
