#include <string>

import commandline;
import benchmark;
import logger;

int main(const int argc, const char* const * argv)
{
	const commandline::parser commandline_parser(argc, argv);

	commandline_parser.parse();

	const std::string_view program_name = argv[0];

	logger logger(program_name);

	logger.initialize();

	skeletonization_benchmark::manager manager;

	manager.register_all();

	manager.run_all();

	manager.show_results();

	return 0;
}
