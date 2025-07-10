module;

#include "CLI/CLI.hpp"

export module commandline:parser;

import :arguments;

namespace commandline
{
	export class parser
	{
	public:
		explicit parser(const int argc, const char* const* argv) : argc_(argc), argv_(argv)
		{
		}

		arguments parse() const
		{
			arguments commandline_arguments;

			CLI::App app{"Skeletonization benchmark"};

			app.add_flag("-v,--verbose", commandline_arguments.verbose, "Enable verbose output");

			try
			{
				app.parse(argc_, argv_);
			}
			catch (const CLI::ParseError& e)
			{
				std::exit(app.exit(e));
			}

			return commandline_arguments;
		}

	private:
		const int argc_;
		const char* const* argv_;
	};
}
