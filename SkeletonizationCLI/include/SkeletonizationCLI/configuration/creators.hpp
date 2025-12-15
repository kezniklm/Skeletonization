#pragma once

#include <string>
#include <vector>

#include "SkeletonizationCore/skeletonizer/skeletonizer.hpp"

namespace configuration
{
	std::vector<skeletonizer_creator> make_algorithm_creators(skeletonizer::skeletonizer_type skeletonizer_type,
	                                                          const std::string& algorithm);
}
