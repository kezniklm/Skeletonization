#pragma once

#include "arguments.hpp"

namespace commandline
{
	class parser
	{
	public:
		parser(int argc, const char* const* argv);

		/**
		 * @brief Parse command-line arguments.
		 * @return Parsed arguments object.
		 */
		[[nodiscard]] arguments parse() const;

	private:
		const int argc_;
		const char* const* argv_;
	};
}
