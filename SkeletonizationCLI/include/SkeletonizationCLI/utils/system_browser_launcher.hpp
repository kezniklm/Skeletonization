/**
*
* @file system_browser_launcher.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares system browser launcher implementation.
*
* This file defines default system browser launcher implementation.
*
* Main responsibilities:
* - open URLs in system default browser
* - implement browser launcher interface contract
* - provide platform-specific launch behavior
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include "SkeletonizationCLI/interfaces/i_browser_launcher.hpp"

namespace cli::utils
{
	/**
	 * @class system_browser_launcher
	 * @brief Default implementation of i_browser_launcher.
	 *
	 * Uses platform-specific system commands to open URLs in the
	 * default web browser (ShellExecute on Windows, open on macOS,
	 * xdg-open on Linux).
	 */
	class system_browser_launcher final : public interfaces::i_browser_launcher
	{
	public:
		/**
		 * @brief Opens URL in system default browser.
		 *
		 * @param url URL to open.
		 * @return True when launch succeeds.
		 */
		[[nodiscard]] bool open(const std::string& url) override;
	};
}
