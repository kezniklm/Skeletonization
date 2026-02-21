#include "SkeletonizationCLI/configuration/loader.hpp"

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

#include "glog/logging.h"

#include "SkeletonizationCLI/exceptions/configuration_exception.hpp"
#include "SkeletonizationCore/configuration/types.hpp"
#include "SkeletonizationCore/skeletonizer/algorithm_factory.hpp"
#include "SkeletonizationCore/skeletonizer/skeletonizer.hpp"
#include "SkeletonizationCore/extensions/string.hpp"
#include <SkeletonizationCLI/configuration/parser.hpp>

namespace configuration
{
	using configuration::image_benchmark_metadata;
	using configuration::skeletonizer_config;

	std::vector<image_benchmark_metadata> configuration_loader::load(const std::string& filename) const
	{
		std::ifstream file(filename);

		if (!file)
		{
			throw cli::exceptions::configuration_file_not_found_exception(std::filesystem::path(filename));
		}

		rapidjson::IStreamWrapper isw(file);
		rapidjson::Document document;
		document.ParseStream(isw);

		if (document.HasParseError())
		{
			throw cli::exceptions::configuration_parse_exception(
				filename, "JSON parse error code: " + std::to_string(document.GetParseError()));
		}

		if (!document.IsArray() || document.Empty())
		{
			throw cli::exceptions::configuration_validation_exception(
				"Configuration file must contain a non-empty array of skeletonizers");
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

	std::vector<image_benchmark_metadata> configuration_loader::load(
		const std::vector<skeletonizer_config>& skeletonizer_configurations) const
	{
		std::vector<image_benchmark_metadata> all_entries;

		for (const auto& [name, path, skeletonizers] : skeletonizer_configurations)
		{
			image_benchmark_metadata meta;
			meta.name = name;
			meta.path = path;

			for (const auto& [type, algorithm] : skeletonizers)
			{
				try
				{
					meta.variants.push_back({type, algorithm});

					process_skeletonizer_variant(type, algorithm, meta);
				}
				catch (const std::exception& e)
				{
					LOG(WARNING) << "Skipping skeletonizer variant (" << type << ":" << algorithm
						<< ") in entry '" << meta.name << "': " << e.what();
				}
			}

			if (!meta.skeletonizers.empty())
			{
				all_entries.push_back(std::move(meta));
			}
			else
			{
				LOG(WARNING) << "Skipping entry '" << meta.name
					<< "': no valid skeletonizers after filtering invalid variants.";
			}
		}

		return all_entries;
	}
}
