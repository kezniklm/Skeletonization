#pragma once

#include "SkeletonizationWorker/configuration.hpp"

namespace worker::infrastructure::platform
{
	void install_console_ctrl_handler(configuration::dependency_injection::cancellation_token_t& token);
}
