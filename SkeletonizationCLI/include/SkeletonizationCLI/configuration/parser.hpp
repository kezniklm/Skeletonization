/**
*
* @file parser.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares configuration parsing services.
 *
 * This file defines JSON parsing helpers for benchmark configuration input.
 *
 * Main responsibilities:
 * - parse configuration JSON documents
 * - validate configuration structure fields
 * - produce typed configuration models
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <string>

#include <rapidjson/document.h>

#include "SkeletonizationCore/configuration/types.hpp"

namespace configuration
{
	/**
	 * @brief Parses one skeletonizer variant object from JSON.
	 * @param json_variant JSON object with type and algorithm fields.
	 * @return Parsed skeletonizer variant.
	 */
	skeletonizer_variant parse_variant(const rapidjson::Value& json_variant);

	/**
	 * @brief Parses one image benchmark entry from configuration JSON.
	 * @param entry JSON object describing one benchmark image entry.
	 * @return Parsed benchmark metadata entry.
	 */
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
