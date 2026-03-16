/**
*
* @file types.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Defines configuration and creator types for skeletonizer composition.
*
* This header declares shared data structures used to represent configured
* algorithm variants and image benchmark metadata across modules.
*
* Main responsibilities:
* - define creator callback aliases for algorithm instantiation
* - declare configuration models for skeletonizer variants
* - store per-image benchmark metadata and prepared creators
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "SkeletonizationCore/skeletonizer/skeletonizer.hpp"

namespace configuration
{
	/// Creator callback that instantiates a skeletonizer implementation.
	using skeletonizer_creator = std::function<std::unique_ptr<skeletonizer::skeletonizer<>>()>;

	/// Mapping from backend type to available skeletonizer creators.
	using skeletonizer_map = std::map<skeletonizer::skeletonizer_type, std::vector<skeletonizer_creator>>;

	/**
	 * @class skeletonizer_variant
	 * @brief Represents a configured algorithm variant.
	 *
	 * This structure stores backend type and algorithm identifier specified in
	 * configuration input.
	 */
	struct skeletonizer_variant
	{
		/// Backend type name.
		std::string type;
		/// Algorithm name.
		std::string algorithm;
	};

	/**
	 * @class skeletonizer_config
	 * @brief Represents configuration for one input image.
	 *
	 * This structure describes an image target and selected algorithm variants.
	 */
	struct skeletonizer_config
	{
		/// Human-readable image label.
		std::string name;
		/// Image file path.
		std::string path;
		/// Requested algorithm variants.
		std::vector<skeletonizer_variant> skeletonizers;
	};

	/**
	 * @class image_benchmark_metadata
	 * @brief Stores prepared metadata for benchmark execution.
	 *
	 * This structure combines configuration values and resolved creator maps used
	 * by benchmark runners.
	 */
	struct image_benchmark_metadata
	{
		/// Human-readable image label.
		std::string name;
		/// Image file path.
		std::string path;
		/// Requested algorithm variants.
		std::vector<skeletonizer_variant> variants;
		/// Resolved creators grouped by backend type.
		skeletonizer_map skeletonizers;
	};
}
