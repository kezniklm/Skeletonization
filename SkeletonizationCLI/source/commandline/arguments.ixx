module;

#include <string>

export module commandline:arguments;

namespace commandline
{
	export struct arguments
	{
		std::string configuration_path = "../skeletonizer_config.json";

		unsigned int number_of_benchmark_iterations = 10000;
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
