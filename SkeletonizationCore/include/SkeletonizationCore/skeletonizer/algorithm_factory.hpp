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
	class algorithm_factory
	{
	public:
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

		[[nodiscard]] static std::vector<std::string_view> available_algorithms()
		{
			std::vector<std::string_view> names;

			configuration::for_each_algorithm([&](const auto& entry)
			{
				names.push_back(entry.name);
			});

			return names;
		}

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
		[[nodiscard]] static std::string normalize(const std::string& name)
		{
			std::string result = to_lower(name);

			std::ranges::replace(result, ' ', '_');
			std::ranges::replace(result, '-', '_');

			return result;
		}
	};
}
