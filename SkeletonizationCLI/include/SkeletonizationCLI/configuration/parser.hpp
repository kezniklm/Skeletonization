#pragma once

#include <string>

#include <rapidjson/document.h>

namespace configuration
{
	skeletonizer::skeletonizer_type parse_type(std::string& skeletonizer_type_string);

	skeletonizer_variant parse_variant(const rapidjson::Value& json_variant);

	image_benchmark_metadata parse_image_entry(const rapidjson::Value& entry);
}
