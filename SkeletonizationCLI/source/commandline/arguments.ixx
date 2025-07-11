module;

#include <string>

export module commandline:arguments;

namespace commandline
{
	export struct arguments
	{
		std::string configuration_path = "../skeletonizer_config.json";
	};
}
