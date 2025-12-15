#include "SkeletonizationCLI/commandline/arguments.hpp"

namespace
{
	commandline::arguments global_arguments_instance;
}

commandline::arguments& global_arguments()
{
	return global_arguments_instance;
}
