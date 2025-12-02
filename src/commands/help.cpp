#include "./help.hpp"

void suffer::commands::help(std::vector<std::string>& arguments) {
    if (arguments.size() < 1) {
        std::cout << suffer::utils::io::red("     _______. __    __   _______  _______  _______ .______      \n    /       ||  |  |  | |   ____||   ____||   ____||   _  \\     \n   |   (----`|  |  |  | |  |__   |  |__   |  |__   |  |_)  |    \n    \\   \\    |  |  |  | |   __|  |   __|  |   __|  |      /     \n.----)   |   |  `--'  | |  |     |  |     |  |____ |  |\\  \\----.\n|_______/     \\______/  |__|     |__|     |_______|| _| `._____|\n||\t\t\t");
        std::cout << suffer::utils::io::magenta("C++ package manager\n");
        std::cout << suffer::utils::io::red("||\n");
        std::cout << suffer::utils::io::cInfo() << suffer::utils::io::cyan(" help\t") << suffer::utils::io::red("Runs this command. Try ") << suffer::utils::io::commandString("suffer help import\n");
        std::cout << suffer::utils::io::cInfo() << suffer::utils::io::cyan(" install\t") << suffer::utils::io::red("Install a package to the registry.\n");
        std::cout << suffer::utils::io::cInfo() << suffer::utils::io::cyan(" import\t") << suffer::utils::io::red("Imports a package to the current working directory.\n");
        std::cout << suffer::utils::io::cInfo() << suffer::utils::io::cyan(" list\t") << suffer::utils::io::red("Displays package information.\n");
        std::cout << suffer::utils::io::cInfo() << suffer::utils::io::cyan(" uninstall\t") << suffer::utils::io::red("Uninstalls a package.") << "\n";
        std::cout << suffer::utils::io::cInfo() << suffer::utils::io::cyan(" clean\t") << suffer::utils::io::red("Deletes suffer's cache.") << "\n";
        std::cout << suffer::utils::io::cInfo() << suffer::utils::io::cyan(" init\t") << suffer::utils::io::red("Generates the structure for a project") << "\n";
        std::cout << suffer::utils::io::cInfo() << suffer::utils::io::cyan(" regen\t") << suffer::utils::io::red("Regenerates the Makefile for a project") << "\n";
        return;
    }
    
    try {
        switch(SWITCHED[arguments[0]]) {
            case HELP:
                std::cout << suffer::utils::io::cInfo() << suffer::utils::io::cyan(" help\n");
                std::cout << suffer::utils::io::red("\tRuns this command.\n\n");
                std::cout << suffer::utils::io::magenta("\tExample usage:\n\t\t");
                std::cout << suffer::utils::io::commandString("suffer help import ") << suffer::utils::io::cyan("// Displays information about the import command\n\t\t");
                std::cout << suffer::utils::io::commandString("suffer help ") << suffer::utils::io::cyan("// Displays all commands") << "\n";
                break;
            case INSTALL:
                std::cout << suffer::utils::io::cInfo() << suffer::utils::io::cyan(" install\n");
                std::cout << suffer::utils::io::red("\tInstalls a package to the repository.\n");
                std::cout << suffer::utils::io::red("\tYou may either install from a github link, or from a local directory.\n\n");
                std::cout << suffer::utils::io::magenta("\tExample usage:\n\t\t");
                std::cout << suffer::utils::io::commandString("suffer install ") << suffer::utils::io::green("https://github/author/repository") << suffer::utils::io::cyan(" // Installs from github\n\t\t");
                std::cout << suffer::utils::io::commandString("suffer install --local ") << suffer::utils::io::cyan("// Installs from a local directory\n\n");
                std::cout << suffer::utils::io::red("\tNOTE: ") << suffer::utils::io::yellow(" When installing locally, you must install from the root directory of the library you wish to install.") << "\n";
                break;
            case IMPORT:
                std::cout << suffer::utils::io::cInfo() << suffer::utils::io::cyan(" import\n");
                std::cout << suffer::utils::io::red("\tImports a package to the current working directory.\n\n");
                std::cout << suffer::utils::io::magenta("\tExample usage:\n\t\t");
                std::cout << suffer::utils::io::commandString("suffer import ") << suffer::utils::io::cyan("libname") << suffer::utils::io::cyan(" // Imports the library \"libname\"\n\n");
                std::cout << suffer::utils::io::red("\tNOTE: ") << suffer::utils::io::yellow(" This tool always assumes that any time you are importing, it is to the current working directory.") << "\n";
                break;
            case LIST:
                std::cout << suffer::utils::io::cInfo() << suffer::utils::io::cyan(" list\n");
                std::cout << suffer::utils::io::red("\tLists information about packages.\n\n");
                std::cout << suffer::utils::io::magenta("\tExample usage:\n\t\t");
                std::cout << suffer::utils::io::commandString("suffer list ") << suffer::utils::io::cyan(" // Lists all libraries\n\t\t");
                std::cout << suffer::utils::io::commandString("suffer list ") << suffer::utils::io::cyan("libname") << suffer::utils::io::cyan(" // Lists information about the library \"libname\"") << "\n";
                break;
            case UNINSTALL:
                std::cout << suffer::utils::io::cInfo() << suffer::utils::io::cyan(" uninstall\n");
                std::cout << suffer::utils::io::red("\tRemoves a package from the registry.\n\n");
                std::cout << suffer::utils::io::magenta("\tExample usage:\n\t\t");
                std::cout << suffer::utils::io::commandString("suffer uninstall ") << suffer::utils::io::cyan("libname") << suffer::utils::io::cyan(" // Removes the library \"libname\"") << "\n";
                break;
            case CLEAN:
                std::cout << suffer::utils::io::cInfo() << suffer::utils::io::cyan(" clean\n");
                std::cout << suffer::utils::io::red("\tDelete's suffer's cache.\n\n");
                std::cout << suffer::utils::io::magenta("\tExample usage:\n\t\t");
                std::cout << suffer::utils::io::commandString("suffer clean ") << "\n";
                break;
            case INIT:
                std::cout << suffer::utils::io::cInfo() << suffer::utils::io::cyan(" init\n");
                std::cout << suffer::utils::io::red("\tGenerates the structure for a project.\n\n");
                std::cout << suffer::utils::io::magenta("\tExample usage:\n\t\t");
                std::cout << suffer::utils::io::commandString("suffer init ") << "\n";
                break;
            case REGEN:
                std::cout << suffer::utils::io::cInfo() << suffer::utils::io::cyan(" regen\n");
                std::cout << suffer::utils::io::red("\tRegenerates the Makefile for a project\n\n");
                std::cout << suffer::utils::io::magenta("\tExample usage:\n\t\t");
                std::cout << suffer::utils::io::commandString("suffer regen ") << "\n";
                break;
            default:
                std::cerr << suffer::utils::io::error() << " I forgot to put this command in help! Oops!\n";
                exit(EXIT_FAILURE);
                break;
        }
    } catch (std::exception& e) {
        std::cerr << suffer::utils::io::error() << " Command " << suffer::utils::io::dataString(arguments[0]) << " does not exist\n";
        exit(EXIT_FAILURE);
    }
}