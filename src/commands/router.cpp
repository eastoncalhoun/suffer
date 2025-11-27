#include "./router.hpp"

suffer::commands::Router::Router(int argc, char** argv) {
    this->arguments = std::vector<std::string>();

    if (argc == 1) {
        suffer::commands::help(this->arguments);
        exit(EXIT_SUCCESS);
    }

    this->command = argv[1];

    if (argc > 2) {
        for (int i = 2; i < argc; i++) {
            arguments.push_back(argv[i]);
        }
    }
}

void suffer::commands::Router::route() {
    switch (SWITCHED[this->command]) {
        case HELP:
            suffer::commands::help(this->arguments);
            break;
        case INSTALL:
            suffer::commands::install(this->arguments);
            break;
        case IMPORT:
            suffer::commands::import(this->arguments);
            break;
        case LIST:
            suffer::commands::list(this->arguments);
            break;
        case UNINSTALL:
            suffer::commands::uninstall(this->arguments);
            break;
        default:
            std::cout << suffer::utils::io::info() << " I want to default on my credit card\n";
            break;
    }
}