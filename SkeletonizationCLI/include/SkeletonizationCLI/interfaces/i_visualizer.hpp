#pragma once

#include <cstdint>
#include <filesystem>

namespace cli::interfaces
{
	/**
	 * @brief Interface for displaying visualization results.
	 *
	 * Abstracts the visualization mechanism to allow different
	 * display methods (web server, desktop app, etc.).
	 */
	class i_visualizer
	{
	public:
		virtual ~i_visualizer() = default;

		/**
		 * @brief Display the visualization.
		 * @param web_root Path to the web root directory or index file.
		 * @param port Port number for the visualization server.
		 */
		virtual void show(const std::filesystem::path& web_root,
		                  std::uint16_t port = 8787) = 0;

	protected:
		i_visualizer() = default;
		i_visualizer(const i_visualizer&) = default;
		i_visualizer& operator=(const i_visualizer&) = default;
		i_visualizer(i_visualizer&&) = default;
		i_visualizer& operator=(i_visualizer&&) = default;
	};
}
