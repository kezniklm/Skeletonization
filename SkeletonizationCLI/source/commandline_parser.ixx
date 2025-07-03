module;

#include <cstdlib>

#include "CLI/CLI.hpp"

export module commandline;

export struct commandline_options
{
	bool verbose = false;
};

export class commandline_parser
{
public:
	explicit commandline_parser(const int argc, const char *const *argv) : argc_(argc), argv_(argv)
	{
	}

	commandline_options parse() const
	{
		commandline_options options;

		CLI::App app{"Skeletonization benchmark"};

		app.add_flag("-v,--verbose", options.verbose, "Enable verbose output");

		try
		{
			app.parse(argc_, argv_);
		}
		catch (const CLI::ParseError &e)
		{
			std::exit(app.exit(e));
		}

		return options;
	}

private:
	const int argc_;
	const char *const *argv_;
};
