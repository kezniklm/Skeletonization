#pragma once

#include <string>
#include <vector>

#include <SkeletonizationCore/configuration/types.hpp>

namespace configuration
{
	std::vector<image_benchmark_metadata> load_skeletonizer_configuration(const std::string& filename);

	std::vector<image_benchmark_metadata> load_skeletonizer_configuration(
		const std::vector<skeletonizer_config>& skeletonizer_configurations);
}
