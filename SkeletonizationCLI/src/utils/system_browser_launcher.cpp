/**
*
* @file system_browser_launcher.cpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Implements system browser launch helpers.
*
* This file implements platform-specific system browser launch behavior.
*
* Main responsibilities:
* - open URLs in default system browser
* - handle platform-specific launch commands
* - report launch failures to callers
*
* @version 1.0
* @date 2026-03-16
*/

#include "SkeletonizationCLI/utils/system_browser_launcher.hpp"

#include <cstdlib>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <shellapi.h>
#endif

#include "glog/logging.h"

namespace cli::utils
{
	bool system_browser_launcher::open(const std::string& url)
	{
#ifdef _WIN32
		// On Windows, use ShellExecuteA for more reliable browser launching
		const auto result = ShellExecuteA(
			nullptr,
			"open",
			url.c_str(),
			nullptr,
			nullptr,
			SW_SHOWNORMAL);

		if (result <= reinterpret_cast<HINSTANCE>(32))
		{
			LOG(ERROR) << "Failed to open browser with URL: " << url;
			return false;
		}
		return true;

#elif defined(__APPLE__)
		// On macOS, use the 'open' command
		const auto cmd = "open \"" + url + "\"";
		const auto return_code = std::system(cmd.c_str());

		if (return_code != 0)
		{
			LOG(ERROR) << "Failed to open browser with URL: " << url
				<< ". Return code: " << return_code;
			return false;
		}
		return true;

#else
		// On Linux and other Unix-like systems, use xdg-open
		const auto cmd = "xdg-open \"" + url + "\"";
		const auto return_code = std::system(cmd.c_str());

		if (return_code != 0)
		{
			LOG(ERROR) << "Failed to open browser with URL: " << url
				<< ". Return code: " << return_code;
			return false;
		}
		return true;
#endif
	}
}
