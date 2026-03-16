/**
*
* @file i_browser_launcher.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares browser launcher interface.
*
* This file defines the contract for opening URLs in a system browser.
*
* Main responsibilities:
* - open URLs in platform browser implementations
* - abstract browser launching from caller logic
* - enable testable launcher substitutions
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <string>

namespace cli::interfaces
{
	/**
	 * @class i_browser_launcher
	 * @brief Interface for launching web browsers.
	 *
	 * Abstracts platform-specific browser launching mechanisms,
	 * enabling testability and platform independence.
	 */
	class i_browser_launcher
	{
	public:
		/**
		 * @brief Destroys the browser launcher interface.
		 */
		virtual ~i_browser_launcher() = default;

		/**
		 * @brief Open a URL in the default system browser.
		 * @param url The URL to open.
		 * @return true if the launcher command executed successfully.
		 */
		[[nodiscard]] virtual bool open(const std::string& url) = 0;

	protected:
		i_browser_launcher() = default;
		i_browser_launcher(const i_browser_launcher&) = default;
		i_browser_launcher& operator=(const i_browser_launcher&) = default;
		i_browser_launcher(i_browser_launcher&&) = default;
		i_browser_launcher& operator=(i_browser_launcher&&) = default;
	};
}
