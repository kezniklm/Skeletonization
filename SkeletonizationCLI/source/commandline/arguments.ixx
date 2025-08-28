module;

#include <string>
#include <vector>

export module commandline:arguments;

import configuration;

namespace commandline
{
	constexpr std::string_view default_configuration_path = "../skeletonizer_config.json";

	export struct arguments
	{
		std::string configuration_path;

		unsigned int number_of_benchmark_iterations = 1000;

		std::vector<skeletonizer_config> skeletonizer_configuration;
	};
}


namespace
{
	commandline::arguments global_arguments_instance;
}

export commandline::arguments& global_arguments()
{
	return global_arguments_instance;
}
