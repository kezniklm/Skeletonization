module;

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include "glog/logging.h"

export module configuration:loader;

import skeletonizer_cpu;
import skeletonizer_gpu;
import skeletonizer;

using skeletonizer_creator = std::function<std::unique_ptr<skeletonizer::skeletonizer>()>;

using skeletonizer_map = std::map<skeletonizer::skeletonizer_type, std::vector<skeletonizer_creator>>;

export struct skeletonizer_variant
{
	std::string type;
	std::string algorithm;
};

export struct skeletonizer_config
{
	std::string name;
	std::string path;
	std::vector<skeletonizer_variant> skeletonizers;
};

export struct image_benchmark_metadata
{
	std::string name;
	std::string path;
	std::vector<skeletonizer_variant> variants;
	skeletonizer_map skeletonizers;
};

namespace configuration
{
	inline skeletonizer::skeletonizer_type parse_type(std::string& skeletonizer_type_string)
	{
		std::ranges::transform(skeletonizer_type_string,
		                       skeletonizer_type_string.begin(), tolower);

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
			return skeletonizer::skeletonizer_type::gpu;
		}

		throw std::runtime_error("Unknown skeletonizer type: " + skeletonizer_type_string);
	}

	inline std::vector<skeletonizer_creator> make_algorithm_creators(
		const skeletonizer::skeletonizer_type skeletonizer_type,
		const std::string& algorithm)
	{
		std::vector<skeletonizer_creator> creators;

		if (algorithm == "zhang_suen")
		{
			if (skeletonizer_type == skeletonizer::skeletonizer_type::cpu)
			{
				creators.push_back([]
				{
					return std::make_unique<skeletonizer::cpu::algorithms::zhang_suen_cpu>();
				});
			}
			else if (skeletonizer_type == skeletonizer::skeletonizer_type::thread)
			{
				creators.push_back([]
				{
					return std::make_unique<skeletonizer::cpu::algorithms::zhang_suen_cpu_threads>();
				});
			}
			else if (skeletonizer_type == skeletonizer::skeletonizer_type::gpu)
			{
				creators.push_back([]
				{
					return std::make_unique<skeletonizer::gpu::algorithms::zhang_suen_gpu>();
				});
			}
		}
		else if (algorithm == "guo_hall")
		{
			if (skeletonizer_type == skeletonizer::skeletonizer_type::cpu)
			{
				creators.push_back([]
				{
					return std::make_unique<skeletonizer::cpu::algorithms::guo_hall_cpu>();
				});
			}
			else if (skeletonizer_type == skeletonizer::skeletonizer_type::thread)
			{
				creators.push_back([]
				{
					return std::make_unique<skeletonizer::cpu::algorithms::guo_hall_cpu_threads>();
				});
			}
			else if (skeletonizer_type == skeletonizer::skeletonizer_type::gpu)
			{
				creators.push_back([]
				{
					return std::make_unique<skeletonizer::gpu::algorithms::guo_hall_gpu>();
				});
			}
		}
		else if (algorithm == "hesselink_roerdink")
		{
			if (skeletonizer_type == skeletonizer::skeletonizer_type::cpu)
			{
				creators.push_back([]
				{
					return std::make_unique<skeletonizer::cpu::algorithms::hesselink_roerdink_cpu>();
				});
			}
			else if (skeletonizer_type == skeletonizer::skeletonizer_type::thread)
			{
				creators.push_back([]
				{
					return std::make_unique<skeletonizer::cpu::algorithms::hesselink_roerdink_cpu_threads>();
				});
			}
			else if (skeletonizer_type == skeletonizer::skeletonizer_type::gpu)
			{
				creators.push_back([]
				{
					return std::make_unique<skeletonizer::gpu::algorithms::hesselink_roerdink_gpu>();
				});
			}
		}

		else if (algorithm == "kwon_gi_kang")
		{
			if (skeletonizer_type == skeletonizer::skeletonizer_type::cpu)
			{
				creators.push_back([]
				{
					return std::make_unique<skeletonizer::cpu::algorithms::kwon_gi_kang_cpu>();
				});
			}
			else if (skeletonizer_type == skeletonizer::skeletonizer_type::thread)
			{
				creators.push_back([]
				{
					return std::make_unique<skeletonizer::cpu::algorithms::kwon_gi_kang_cpu_threads>();
				});
			}
			else if (skeletonizer_type == skeletonizer::skeletonizer_type::gpu)
			{
				creators.push_back([]
				{
					return std::make_unique<skeletonizer::gpu::algorithms::kwon_gi_kang_gpu>();
				});
			}
		}

		else if (algorithm == "petrosino_salvi")
		{
			if (skeletonizer_type == skeletonizer::skeletonizer_type::cpu)
			{
				creators.push_back([]
				{
					return std::make_unique<skeletonizer::cpu::algorithms::petrosino_salvi_cpu>();
				});
			}
			else if (skeletonizer_type == skeletonizer::skeletonizer_type::thread)
			{
				creators.push_back([]
				{
					return std::make_unique<skeletonizer::cpu::algorithms::petrosino_salvi_thread>();
				});
			}
			else if (skeletonizer_type == skeletonizer::skeletonizer_type::gpu)
			{
				creators.push_back([]
				{
					return std::make_unique<skeletonizer::gpu::algorithms::petrosino_salvi_gpu>();
				});
			}
		}

		if (creators.empty())
		{
			throw std::runtime_error(
				"No valid creator for type=" + to_string(skeletonizer_type) + ", algorithm=" + algorithm);
		}

		return creators;
	}

	inline skeletonizer_variant parse_variant(const rapidjson::Value& json_variant)
	{
		if (!json_variant.HasMember("type") || !json_variant["type"].IsString() ||
			!json_variant.HasMember("algorithm") || !json_variant["algorithm"].IsString())
		{
			throw std::runtime_error("Missing or invalid 'type' or 'algorithm' in variant.");
		}

		return {
			json_variant["type"].GetString(),
			json_variant["algorithm"].GetString()
		};
	}

	inline image_benchmark_metadata parse_image_entry(const rapidjson::Value& entry)
	{
		if (!entry.HasMember("name") || !entry["name"].IsString() ||
			!entry.HasMember("path") || !entry["path"].IsString() ||
			!entry.HasMember("skeletonizers") || !entry["skeletonizers"].IsArray())
		{
			throw std::runtime_error("Missing 'name', 'path', or 'skeletonizers' in entry.");
		}

		image_benchmark_metadata metadata;
		metadata.name = entry["name"].GetString();
		metadata.path = entry["path"].GetString();

		for (const auto& json_variant : entry["skeletonizers"].GetArray())
		{
			auto variant = parse_variant(json_variant);
			metadata.variants.push_back(variant);

			const auto type_enum = parse_type(variant.type);
			const auto creators = make_algorithm_creators(type_enum, variant.algorithm);

			auto& existing = metadata.skeletonizers[type_enum];
			existing.insert(existing.end(),
			                std::make_move_iterator(creators.begin()),
			                std::make_move_iterator(creators.end()));
		}

		return metadata;
	}


	export inline std::vector<image_benchmark_metadata> load_skeletonizer_configuration(const std::string& filename)
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
			LOG(FATAL) << "Configuration file must contain a list of skeletonizers.";
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

	export inline std::vector<image_benchmark_metadata> load_skeletonizer_configuration(
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

				const auto type_enum = parse_type(const_cast<std::string&>(type));

				const auto creators = make_algorithm_creators(type_enum, algorithm);

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
