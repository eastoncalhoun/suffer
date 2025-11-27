#include "./router.hpp"

suffer::commands::Router::Router(int argc, char** argv) {
    this->arguments = std::vector<std::string>();

    if (argc != 1) {
        for (int i = 1; i < argc; i++) {
            arguments.push_back(argv[i]);
        }
    } else {
        arguments.push_back("");
    }
}

void suffer::commands::Router::route() {
    const std::string command = this->arguments[0];

    if (command == "install") {
        return suffer::commands::install(this->arguments);
    } else if (command == "import") {
        return suffer::commands::import(this->arguments);
    }
}