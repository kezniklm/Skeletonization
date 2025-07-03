import commandline;

import benchmark_manager;

int main(const int argc, const char *const *argv)
{
    const commandline_parser commandline_parser(argc, argv);

    const auto commandline_options = commandline_parser.parse();

    benchmark_manager manager;

    manager.register_all();

    manager.run_all();

    manager.show_results();

    return 0;
}
