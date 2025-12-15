#pragma once

namespace commandline
{
	class parser
	{
	public:
		parser(int argc, const char* const* argv);

		void parse() const;

	private:
		const int argc_;
		const char* const* argv_;
	};
}
