/**
*
* @file i_visualizer.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares visualization interface for benchmark results.
*
* This file defines the contract for presenting benchmark visualizations.
*
* Main responsibilities:
* - display generated benchmark visualizations
* - abstract visualization backend from callers
* - support testable visualizer substitutions
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <cstdint>
#include <filesystem>

namespace cli::interfaces
{
	/**
	 * @class i_visualizer
	 * @brief Interface for displaying visualization results.
	 *
	 * Abstracts the visualization mechanism to allow different
	 * display methods (web server, desktop app, etc.).
	 */
	class i_visualizer
	{
	public:
		/**
		 * @brief Destroys the visualizer interface.
		 */
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
