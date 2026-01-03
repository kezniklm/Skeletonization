#include <string>

#include "SkeletonizationCore/logger/logger.hpp"

#include "SkeletonizationWorker/configuration.hpp"
#include "SkeletonizationWorker/dependency_injection.hpp"

#include "SkeletonizationWorker/application/use_cases/worker_loop.hpp"
#include "SkeletonizationWorker/infrastructure/platform/console_ctrl_handler.hpp"

using namespace worker;

int main(const int /*argc*/, const char* const* argv)
{
	const std::string_view program_name = argv[0];

	logger logger(program_name);

	logger.initialize();

	LOG(INFO) << "SkeletonizationWorker started";

	try
	{
		const auto configuration = configuration::configuration::from_environment();

		const auto injector = dependency_injection::configure_dependency_injection(configuration);

		auto& cancellation_token = injector.create<configuration::dependency_injection::cancellation_token_t&>();

		infrastructure::platform::install_console_ctrl_handler(cancellation_token);

		const auto worker_loop = injector.create<application::use_cases::worker_loop>();

		worker_loop.run();

		LOG(INFO) << "SkeletonizationWorker stopped gracefully";

		return EXIT_SUCCESS;
	}
	catch (const std::exception& e)
	{
		LOG(ERROR) << "Fatal exception: " << e.what();

		return EXIT_FAILURE;
	}
	catch (...)
	{
		LOG(ERROR) << "Fatal unknown exception";

		return EXIT_FAILURE;
	}
}
