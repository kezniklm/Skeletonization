/**
*
* @file parser.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares command-line parser services.
*
* This file defines the CLI parser responsible for transforming raw
* argv inputs into structured argument values.
*
* Main responsibilities:
* - store raw process argument arrays
* - parse raw arguments into typed structure
* - validate argument values and options
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include "arguments.hpp"

namespace commandline
{
	/**
	 * @class parser
	 * @brief Parses command-line input into argument structure.
	 */
	class parser
	{
	public:
		/**
		 * @brief Constructs parser from process argument array.
		 *
		 * @param argc Number of command-line arguments.
		 * @param argv Command-line argument values.
		 */
		parser(int argc, const char* const* argv);

		/**
		 * @brief Parse command-line arguments.
		 * @return Parsed arguments object.
		 */
		[[nodiscard]] arguments parse() const;

	private:
		/// Number of command-line arguments.
		const int argc_;
		/// Command-line argument value array.
		const char* const* argv_;
	};
}
