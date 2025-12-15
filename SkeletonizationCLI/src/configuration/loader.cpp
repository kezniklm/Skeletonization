#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

#include "glog/logging.h"

#include "SkeletonizationCore/configuration/types.hpp"
#include <SkeletonizationCLI/configuration/creators.hpp>
#include <SkeletonizationCLI/configuration/parser.hpp>

namespace configuration
{
	std::vector<image_benchmark_metadata> load_skeletonizer_configuration(const std::string& filename)
	{
		std::ifstream file(filename);
		if (!file)
		{
			LOG(FATAL) << "Cannot open configuration file: " << filename;
		}

		rapidjson::IStreamWrapper isw(file);
		rapidjson::Document document;
		document.ParseStream(isw);

		if (document.HasParseError())
		{
			LOG(FATAL) << "Failed to parse JSON file: " << filename;
		}

		if (!document.IsArray() || document.Empty())
		{
			LOG(FATAL)
				<< "Configuration file must contain a list of skeletonizers.";
		}

		std::vector<image_benchmark_metadata> all_entries;

		for (rapidjson::SizeType index = 0; index < document.Size(); ++index)
		{
			try
			{
				all_entries.emplace_back(parse_image_entry(document[index]));
			}
			catch (const std::exception& e)
			{
				LOG(WARNING) << "Skipping entry " << index << ": " << e.what();
			}
		}

		return all_entries;
	}

	std::vector<image_benchmark_metadata> load_skeletonizer_configuration(
		const std::vector<skeletonizer_config>& skeletonizer_configurations)
	{
		std::vector<image_benchmark_metadata> all_entries;

		for (const auto& [name, path, skeletonizers] : skeletonizer_configurations)
		{
			image_benchmark_metadata meta;
			meta.name = name;
			meta.path = path;

			for (const auto& [type, algorithm] : skeletonizers)
			{
				meta.variants.push_back({type, algorithm});

				auto& type_ref = const_cast<std::string&>(type);
				const auto type_enum = parse_type(type_ref);

				auto creators = make_algorithm_creators(type_enum, algorithm);

				auto& existing = meta.skeletonizers[type_enum];
				existing.insert(existing.end(),
				                std::make_move_iterator(creators.begin()),
				                std::make_move_iterator(creators.end()));
			}

			all_entries.push_back(std::move(meta));
		}

		return all_entries;
	}
}
