#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "SkeletonizationCore/skeletonizer/skeletonizer.hpp"

namespace configuration
{
	using skeletonizer_creator = std::function<std::unique_ptr<skeletonizer::skeletonizer<>>()>;

	using skeletonizer_map = std::map<skeletonizer::skeletonizer_type, std::vector<skeletonizer_creator>>;

	struct skeletonizer_variant
	{
		std::string type;
		std::string algorithm;
	};

	struct skeletonizer_config
	{
		std::string name;
		std::string path;
		std::vector<skeletonizer_variant> skeletonizers;
	};

	struct image_benchmark_metadata
	{
		std::string name;
		std::string path;
		std::vector<skeletonizer_variant> variants;
		skeletonizer_map skeletonizers;
	};
}
