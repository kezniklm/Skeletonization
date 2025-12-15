#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "SkeletonizationCore/configuration/types.hpp"

namespace commandline
{
	inline constexpr std::string_view default_configuration_path =
		"../skeletonizer_config.json";

	struct arguments
	{
		std::string configuration_path =
			std::string(default_configuration_path);

		std::string benchmark_out;

		unsigned int number_of_benchmark_iterations = 1000;

		std::vector<configuration::skeletonizer_config> skeletonizer_configuration;

		bool run_image_preprocessing{};
	};
}

commandline::arguments& global_arguments();
