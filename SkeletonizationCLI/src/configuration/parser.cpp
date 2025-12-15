#include <algorithm>
#include <cctype>
#include <ranges>
#include <stdexcept>
#include <string>

#include <rapidjson/document.h>

#include "SkeletonizationCore/configuration/types.hpp"
#include "SkeletonizationCore/skeletonizer/skeletonizer.hpp"
#include "SkeletonizationCLI/configuration/creators.hpp"

namespace configuration
{
	skeletonizer::skeletonizer_type parse_type(std::string& skeletonizer_type_string)
	{
		std::ranges::transform(skeletonizer_type_string, skeletonizer_type_string.begin(), [](unsigned char c)
		{
			return static_cast<char>(std::tolower(c));
		});

		if (skeletonizer_type_string == "cpu")
		{
			return skeletonizer::skeletonizer_type::cpu;
		}
		if (skeletonizer_type_string == "thread")
		{
			return skeletonizer::skeletonizer_type::thread;
		}
		if (skeletonizer_type_string == "gpu")
		{
#if SKELETONIZATION_WITH_GPU
			return skeletonizer::skeletonizer_type::gpu;
#else
			throw std::runtime_error(
				"GPU requested in configuration but not compiled in.");
#endif
		}

		throw std::runtime_error(
			"Unknown skeletonizer type: " + skeletonizer_type_string);
	}

	skeletonizer_variant parse_variant(const rapidjson::Value& json_variant)
	{
		if (!json_variant.HasMember("type") || !json_variant["type"].IsString() ||
			!json_variant.HasMember("algorithm") || !json_variant["algorithm"].IsString())
		{
			throw std::runtime_error(
				"Missing or invalid 'type' or 'algorithm' in variant.");
		}

		return {
			json_variant["type"].GetString(),
			json_variant["algorithm"].GetString()
		};
	}

	image_benchmark_metadata parse_image_entry(const rapidjson::Value& entry)
	{
		if (!entry.HasMember("name") || !entry["name"].IsString() ||
			!entry.HasMember("path") || !entry["path"].IsString() ||
			!entry.HasMember("skeletonizers") || !entry["skeletonizers"].IsArray())
		{
			throw std::runtime_error(
				"Missing 'name', 'path', or 'skeletonizers' in entry.");
		}

		image_benchmark_metadata metadata;
		metadata.name = entry["name"].GetString();
		metadata.path = entry["path"].GetString();

		for (const auto& json_variant : entry["skeletonizers"].GetArray())
		{
			auto variant = parse_variant(json_variant);
			metadata.variants.push_back(variant);

			auto type_enum = parse_type(variant.type);
			auto creators = make_algorithm_creators(type_enum, variant.algorithm);

			auto& existing = metadata.skeletonizers[type_enum];
			existing.insert(
				existing.end(),
				std::make_move_iterator(creators.begin()),
				std::make_move_iterator(creators.end()));
		}

		return metadata;
	}
}
