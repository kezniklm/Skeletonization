#include <string>

#include "SkeletonizationCore/logger/logger.hpp"

#include "SkeletonizationCLI/commandline/parser.hpp"
#include "SkeletonizationCLI/benchmark/manager.hpp"

int main(const int argc, const char* const * argv)
{
	const std::string_view program_name = argv[0];

	logger logger(program_name);

	logger.initialize();

	const commandline::parser commandline_parser(argc, argv);

	commandline_parser.parse();

	skeletonization_benchmark::manager manager;

	manager.register_all();

	const auto output_json = manager.run_all();

	manager.show_results(output_json);

	return 0;
}
