/**
*
* @file main.cpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Defines the Skeletonization CLI application entry point.
*
* This file initializes logging, parses command-line arguments, wires
* dependencies, and executes benchmark workflows.
*
* Main responsibilities:
* - initialize application logging and diagnostics
* - parse command-line arguments for benchmark execution
* - configure dependency injection graph and run benchmarks
* - report benchmark results and handle fatal errors
*
* @version 1.0
* @date 2026-03-16
*/

#include <exception>
#include <string>

#include "glog/logging.h"

#include "SkeletonizationCLI/benchmark/manager.hpp"
#include "SkeletonizationCLI/commandline/parser.hpp"
#include "SkeletonizationCLI/dependency_injection/dependency_injection.hpp"
#include "SkeletonizationCore/logger/logger.hpp"

/**
 * @brief Runs the Skeletonization CLI benchmark workflow.
 *
 * @param argc Number of command-line arguments.
 * @param argv Command-line argument values.
 * @return Process exit code indicating success or failure.
 */
int main(const int argc, const char* const * argv)
{
	const std::string_view program_name = argv[0];

	logger logger(program_name);

	logger.initialize();

	try
	{
		const commandline::parser commandline_parser(argc, argv);

		const auto arguments = commandline_parser.parse();

		const auto injector = cli::dependency_injection::configure(arguments);

		auto manager = injector.create<skeletonization_benchmark::manager>();

		manager.register_all();

		const auto output_json = manager.run_all();

		manager.show_results(output_json);
	}
	catch (const std::exception& e)
	{
		LOG(ERROR) << "Fatal error: " << e.what();

		return EXIT_FAILURE;
	}
	catch (...)
	{
		LOG(ERROR) << "Unknown fatal error occurred.";

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
