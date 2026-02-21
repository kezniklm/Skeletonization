#pragma once

#include <string>

#include <rapidjson/document.h>

#include "SkeletonizationCore/configuration/types.hpp"

namespace configuration
{
	skeletonizer_variant parse_variant(const rapidjson::Value& json_variant);

	image_benchmark_metadata parse_image_entry(const rapidjson::Value& entry);

	/**
	 * @brief Process a skeletonizer variant (type + algorithm) and add it to metadata.
	 * @param type The skeletonizer type (e.g., "cpu", "thread", "gpu")
	 * @param algorithm The algorithm name (e.g., "zhang-suen")
	 * @param metadata The metadata to populate with skeletonizers
	 * @throws configuration_validation_exception if type is unknown or GPU not compiled
	 */
	void process_skeletonizer_variant(
		const std::string& type,
		const std::string& algorithm,
		image_benchmark_metadata& metadata);
}
