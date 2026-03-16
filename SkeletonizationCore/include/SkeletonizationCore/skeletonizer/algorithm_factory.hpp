/**
*
* @file algorithm_factory.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Creates skeletonizer algorithm instances from registered metadata.
*
* This header defines a static factory that resolves algorithm names to
* runtime creators for selected backend types. It also exposes helpers for
* querying support and listing available algorithm identifiers.
*
* Main responsibilities:
* - resolve algorithm names to creator functions
* - instantiate skeletonizer implementations by backend type
* - provide runtime support and discovery helpers
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "SkeletonizationCore/skeletonizer/algorithm_registry.hpp"

namespace skeletonizer
{
	/**
	 * @class algorithm_factory
	 * @brief Provides static factory operations for skeletonizer algorithms.
	 *
	 * This class uses registered algorithm entries to create implementations and
	 * query backend support at runtime.
	 */
	class algorithm_factory
	{
	public:
		/**
		 * @brief Creates a skeletonizer instance for an algorithm and backend type.
		 *
		 * @param algorithm_name Algorithm identifier to resolve.
		 * @param type Requested backend type.
		 * @return Created skeletonizer instance or null when unavailable.
		 */
		[[nodiscard]] static std::unique_ptr<skeletonizer<>> create(const std::string& algorithm_name,
		                                                            const skeletonizer_type type =
			                                                            skeletonizer_type::cpu)
		{
			const auto normalized = normalize(algorithm_name);

			std::unique_ptr<skeletonizer<>> result;

			configuration::for_each_algorithm([&](const auto& entry)
			{
				if (result)
				{
					return;
				}

				if (normalized != entry.name)
				{
					return;
				}

				std::vector<configuration::skeletonizer_creator> creators;

				entry.push_creators_for_type(type, creators);

				if (!creators.empty())
				{
					result = creators.front()();
				}
			});

			return result;
		}

		/**
		 * @brief Returns all creators for an algorithm and backend type.
		 *
		 * @param algorithm_name Algorithm identifier to resolve.
		 * @param type Requested backend type.
		 * @return List of creator callbacks.
		 */
		[[nodiscard]] static std::vector<configuration::skeletonizer_creator> creators_for(
			const std::string& algorithm_name,
			const skeletonizer_type type)
		{
			const auto normalized = normalize(algorithm_name);

			std::vector<configuration::skeletonizer_creator> creators;

			configuration::for_each_algorithm([&](const auto& entry)
			{
				if (normalized == entry.name)
				{
					entry.push_creators_for_type(type, creators);
				}
			});

			if (creators.empty())
			{
				throw std::runtime_error(
					"No valid creator for type=" + to_string(type) +
					", algorithm=" + algorithm_name);
			}

			return creators;
		}

		/**
		 * @brief Lists all registered algorithm names.
		 *
		 * @return Collection of available algorithm identifiers.
		 */
		[[nodiscard]] static std::vector<std::string_view> available_algorithms()
		{
			std::vector<std::string_view> names;

			configuration::for_each_algorithm([&](const auto& entry)
			{
				names.push_back(entry.name);
			});

			return names;
		}

		/**
		 * @brief Checks whether an algorithm supports a backend type.
		 *
		 * @param algorithm_name Algorithm identifier.
		 * @param type Backend type to test.
		 * @return True when at least one creator exists for the type.
		 */
		[[nodiscard]] static bool supports(const std::string& algorithm_name, const skeletonizer_type type)
		{
			const auto normalized = normalize(algorithm_name);

			bool found = false;

			configuration::for_each_algorithm([&](const auto& entry)
			{
				if (found)
				{
					return;
				}

				if (normalized != entry.name)
				{
					return;
				}

				std::vector<configuration::skeletonizer_creator> creators;

				entry.push_creators_for_type(type, creators);

				found = !creators.empty();
			});

			return found;
		}

	private:
		/**
		 * @brief Normalizes algorithm names for lookup.
		 *
		 * @param name Raw algorithm name.
		 * @return Lowercase algorithm key with unified separators.
		 */
		[[nodiscard]] static std::string normalize(const std::string& name)
		{
			std::string result = to_lower(name);

			std::ranges::replace(result, ' ', '_');
			std::ranges::replace(result, '-', '_');

			return result;
		}
	};
}
