#pragma once

#include <cstdint>
#include <filesystem>
#include <string>

#include "cli_exception.hpp"

namespace cli::exceptions
{
	/**
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

		[[nodiscard]] const std::filesystem::path& root() const noexcept
		{
			return root_;
		}

	private:
		std::filesystem::path root_;
	};

	/**
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

		[[nodiscard]] std::uint16_t port() const noexcept
		{
			return port_;
		}

	private:
		std::uint16_t port_;
	};

	/**
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

		[[nodiscard]] const std::string& url() const noexcept
		{
			return url_;
		}

	private:
		std::string url_;
	};
}
