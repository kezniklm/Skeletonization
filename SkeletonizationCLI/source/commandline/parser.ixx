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

		void parse() const
		{
			auto& [configuration_path, number_of_benchmark_iterations] = global_arguments();

			CLI::App app{"Skeletonization benchmark"};

			app.add_option("-c,--config", configuration_path, "Path to configuration JSON file")
			   ->default_val(configuration_path)
			   ->check(CLI::ExistingFile);

			try
			{
				app.parse(argc_, argv_);
			}
			catch (const CLI::ParseError& e)
			{
				std::exit(app.exit(e));
			}
		}

	private:
		const int argc_;
		const char* const* argv_;
	};
}
