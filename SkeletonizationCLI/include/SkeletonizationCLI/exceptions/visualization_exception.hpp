/**
*
* @file visualization_exception.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares visualization-related exception type.
*
* This file defines visualization exception hierarchy for web root,
* server startup, and browser launch failures.
*
* Main responsibilities:
* - define base visualization exception type
* - define web root and server startup exceptions
* - define browser launch exception type
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <cstdint>
#include <filesystem>
#include <string>

#include "cli_exception.hpp"

namespace cli::exceptions
{
	/**
	 * @class visualization_exception
	 * @brief Exception thrown when visualization operations fail.
	 */
	class visualization_exception : public cli_exception
	{
	public:
		explicit visualization_exception(const std::string& message)
			: cli_exception(message)
		{
		}

		explicit visualization_exception(const char* message)
			: cli_exception(message)
		{
		}
	};

	/**
	 * @class web_root_not_found_exception
	 * @brief Exception thrown when the web root directory does not exist.
	 */
	class web_root_not_found_exception final : public visualization_exception
	{
	public:
		explicit web_root_not_found_exception(const std::filesystem::path& root)
			: visualization_exception("Visualizer root directory does not exist: " + root.string())
			  , root_(root)
		{
		}

		/**
		 * @brief Returns missing web root path.
		 *
		 * @return Missing web root path.
		 */
		[[nodiscard]] const std::filesystem::path& root() const noexcept
		{
			return root_;
		}

	private:
		/// Missing web root path.
		std::filesystem::path root_;
	};

	/**
	 * @class server_startup_exception
	 * @brief Exception thrown when server startup fails.
	 */
	class server_startup_exception final : public visualization_exception
	{
	public:
		server_startup_exception(const std::uint16_t port, const std::string& reason)
			: visualization_exception(
				  "Failed to start visualization server on port " +
				  std::to_string(port) + ": " + reason)
			  , port_(port)
		{
		}

		/**
		 * @brief Returns port that failed to start.
		 *
		 * @return Failed port value.
		 */
		[[nodiscard]] std::uint16_t port() const noexcept
		{
			return port_;
		}

	private:
		/// Failed server port.
		std::uint16_t port_;
	};

	/**
	 * @class browser_launch_exception
	 * @brief Exception thrown when browser launch fails.
	 */
	class browser_launch_exception final : public visualization_exception
	{
	public:
		explicit browser_launch_exception(const std::string& url)
			: visualization_exception("Failed to open browser with URL: " + url)
			  , url_(url)
		{
		}

		/**
		 * @brief Returns URL that failed to open.
		 *
		 * @return URL string.
		 */
		[[nodiscard]] const std::string& url() const noexcept
		{
			return url_;
		}

	private:
		/// URL that failed to open.
		std::string url_;
	};
}
