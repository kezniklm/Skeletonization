#pragma once

#include "SkeletonizationCLI/interfaces/i_browser_launcher.hpp"

namespace cli::utils
{
	/**
	 * @brief Default implementation of i_browser_launcher.
	 *
	 * Uses platform-specific system commands to open URLs in the
	 * default web browser (ShellExecute on Windows, open on macOS,
	 * xdg-open on Linux).
	 */
	class system_browser_launcher final : public interfaces::i_browser_launcher
	{
	public:
		[[nodiscard]] bool open(const std::string& url) override;
	};
}
