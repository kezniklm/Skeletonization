/**
*
* @file console_ctrl_handler.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares console control handler utilities for worker shutdown.
 *
 * This file defines platform console handler utilities for cancellation.
 *
 * Main responsibilities:
 * - register console control callbacks
 * - forward cancel signals to cancellation token
 * - support graceful worker shutdown on signal
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include "SkeletonizationWorker/configuration.hpp"

namespace worker::infrastructure::platform
{
	/**
	 * @brief Installs console control handler bound to cancellation token.
	 * @param token Shared cancellation token updated on stop signals.
	 */
	void install_console_ctrl_handler(configuration::dependency_injection::cancellation_token_t& token);
}
