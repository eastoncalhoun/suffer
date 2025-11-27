#include "./router.hpp"

suffer::commands::Router::Router(int argc, char** argv) {
    this->arguments = std::vector<std::string>();

    if (argc == 1) {
        //help command
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
    if (command == "install") {
        return suffer::commands::install(this->arguments);
    } else if (command == "import") {
        return suffer::commands::import(this->arguments);
    } else if (command == "list") {
        return suffer::commands::list(this->arguments);
    }
}