#include "SkeletonizationCLI/configuration/parser.hpp"

#include <algorithm>
#include <ranges>
#include <stdexcept>
#include <string>

#include <rapidjson/document.h>

#include "glog/logging.h"
#include "SkeletonizationCLI/exceptions/configuration_exception.hpp"
#include "SkeletonizationCore/configuration/types.hpp"
#include "SkeletonizationCore/skeletonizer/skeletonizer.hpp"
#include "SkeletonizationCore/skeletonizer/algorithm_factory.hpp"
#include "SkeletonizationCore/extensions/string.hpp"

namespace configuration
{
	skeletonizer_variant parse_variant(const rapidjson::Value& json_variant)
	{
		if (!json_variant.HasMember("type") || !json_variant["type"].IsString() ||
			!json_variant.HasMember("algorithm") || !json_variant["algorithm"].IsString())
		{
			throw cli::exceptions::configuration_validation_exception(
				"Missing or invalid 'type' or 'algorithm' in variant.");
		}

		return {
			json_variant["type"].GetString(),
			json_variant["algorithm"].GetString()
		};
	}

	void process_skeletonizer_variant(
		const std::string& type,
		const std::string& algorithm,
		image_benchmark_metadata& metadata)
	{
		const auto type_enum = skeletonizer::from_string(type);

		if (!type_enum)
		{
#if !SKELETONIZATION_WITH_GPU
			if (equals_ascii(type, "gpu"))
			{
				throw cli::exceptions::configuration_validation_exception(
					"GPU skeletonizer requested but not compiled in. Rebuild with SKELETONIZATION_WITH_GPU=ON");
			}
#endif
			throw cli::exceptions::configuration_validation_exception(
				"Unknown skeletonizer type: " + type + ". Valid types: cpu, thread" +
#if SKELETONIZATION_WITH_GPU
				", gpu"
#else
				""
#endif
			);
		}

		auto creators = skeletonizer::algorithm_factory::creators_for(algorithm, type_enum.value());

		auto& existing = metadata.skeletonizers[type_enum.value()];

		existing.insert(
			existing.end(),
			std::make_move_iterator(creators.begin()),
			std::make_move_iterator(creators.end()));
	}

	image_benchmark_metadata parse_image_entry(const rapidjson::Value& entry)
	{
		if (!entry.HasMember("name") || !entry["name"].IsString() ||
			!entry.HasMember("path") || !entry["path"].IsString() ||
			!entry.HasMember("skeletonizers") || !entry["skeletonizers"].IsArray())
		{
			throw cli::exceptions::configuration_validation_exception(
				"Missing 'name', 'path', or 'skeletonizers' in entry.");
		}

		image_benchmark_metadata metadata;
		metadata.name = entry["name"].GetString();
		metadata.path = entry["path"].GetString();

		for (const auto& json_variant : entry["skeletonizers"].GetArray())
		{
			try
			{
				auto variant = parse_variant(json_variant);

				metadata.variants.push_back(variant);

				process_skeletonizer_variant(variant.type, variant.algorithm, metadata);
			}
			catch (const std::exception& e)
			{
				LOG(WARNING) << "Skipping skeletonizer variant in entry '" << metadata.name
					<< "': " << e.what();
			}
		}

		if (metadata.skeletonizers.empty())
		{
			throw cli::exceptions::configuration_validation_exception(
				"Entry '" + metadata.name + "' has no valid skeletonizers after filtering invalid variants.");
		}

		return metadata;
	}
}
