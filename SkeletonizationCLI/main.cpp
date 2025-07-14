import commandline;
import benchmark;

int main(const int argc, const char* const * argv)
{
	const commandline::parser commandline_parser(argc, argv);

	commandline_parser.parse();

	skeletonization_benchmark::manager manager;

	manager.register_all();

	manager.run_all();

	manager.show_results();

	return 0;
}
