#pragma once

#include <string>

namespace cli::interfaces
{
	/**
	 * @brief Interface for launching web browsers.
	 *
	 * Abstracts platform-specific browser launching mechanisms,
	 * enabling testability and platform independence.
	 */
	class i_browser_launcher
	{
	public:
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
