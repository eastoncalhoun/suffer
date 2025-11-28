#include "./help.hpp"

void suffer::commands::help(std::vector<std::string>& arguments) {
    if (arguments.size() < 1) {
        std::cout << suffer::utils::io::red("     _______. __    __   _______  _______  _______ .______      \n    /       ||  |  |  | |   ____||   ____||   ____||   _  \\     \n   |   (----`|  |  |  | |  |__   |  |__   |  |__   |  |_)  |    \n    \\   \\    |  |  |  | |   __|  |   __|  |   __|  |      /     \n.----)   |   |  `--'  | |  |     |  |     |  |____ |  |\\  \\----.\n|_______/     \\______/  |__|     |__|     |_______|| _| `._____|\n||\n");
        std::cout << suffer::utils::io::cInfo() << suffer::utils::io::yellow(" help\t") << suffer::utils::io::red("Runs this command. Try ") << suffer::utils::io::commandString("suffer help import\n");
        std::cout << suffer::utils::io::cInfo() << suffer::utils::io::yellow(" install\t") << suffer::utils::io::red("Install a package to the registry.\n");
        std::cout << suffer::utils::io::cInfo() << suffer::utils::io::yellow(" import\t") << suffer::utils::io::red("Imports a package to the current working directory.\n");
        std::cout << suffer::utils::io::cInfo() << suffer::utils::io::yellow(" list\t") << suffer::utils::io::red("Displays package information.\n");
        std::cout << suffer::utils::io::cInfo() << suffer::utils::io::yellow(" uninstall\t") << suffer::utils::io::red("Uninstalls a package.") << "\n";
        return;
    }
    
    try {
        switch(SWITCHED[arguments[0]]) {
            case HELP:
                std::cout << suffer::utils::io::cInfo() << suffer::utils::io::yellow(" help\n");
                std::cout << suffer::utils::io::red("\tRuns this command.\n\n");
                std::cout << suffer::utils::io::magenta("\tExample usage:\n\t\t");
                std::cout << suffer::utils::io::commandString("suffer help import ") << suffer::utils::io::cyan("// Displays information about the import command\n\t\t");
                std::cout << suffer::utils::io::commandString("suffer help ") << suffer::utils::io::cyan("// Displays all commands") << "\n";
                break;
            case INSTALL:
                std::cout << suffer::utils::io::cInfo() << suffer::utils::io::yellow(" install\n");
                std::cout << suffer::utils::io::red("\tInstalls a package to the repository.\n");
                std::cout << suffer::utils::io::red("\tYou may either install from a github link, or from a local directory.\n\n");
                std::cout << suffer::utils::io::magenta("\tExample usage:\n\t\t");
                std::cout << suffer::utils::io::commandString("suffer install ") << suffer::utils::io::yellow("https://github/author/repository") << suffer::utils::io::cyan(" // Installs from github\n\t\t");
                std::cout << suffer::utils::io::commandString("suffer install --local ") << suffer::utils::io::cyan("// Installs from a local directory\n\n");
                std::cout << suffer::utils::io::red("\tNOTE: ") << suffer::utils::io::yellow(" When installing locally, you must install from the root directory of the library you wish to install.") << "\n";
                break;
            case IMPORT:
                std::cout << suffer::utils::io::cInfo() << suffer::utils::io::yellow(" import\n");
                std::cout << suffer::utils::io::red("\tImports a package to the current working directory.\n\n");
                std::cout << suffer::utils::io::magenta("\tExample usage:\n\t\t");
                std::cout << suffer::utils::io::commandString("suffer import ") << suffer::utils::io::yellow("libname") << suffer::utils::io::cyan(" // Imports the library \"libname\"\n\n");
                std::cout << suffer::utils::io::red("\tNOTE: ") << suffer::utils::io::yellow(" This tool always assumes that any time you are importing, it is to the current working directory.") << "\n";
                break;
            case LIST:
                std::cout << suffer::utils::io::cInfo() << suffer::utils::io::yellow(" list\n");
                std::cout << suffer::utils::io::red("\tLists information about packages.\n\n");
                std::cout << suffer::utils::io::magenta("\tExample usage:\n\t\t");
                std::cout << suffer::utils::io::commandString("suffer list ") << suffer::utils::io::cyan(" // Lists all libraries\n\t\t");
                std::cout << suffer::utils::io::commandString("suffer list ") << suffer::utils::io::yellow("libname") << suffer::utils::io::cyan(" // Lists information about the library \"libname\"") << "\n";
                break;
            case UNINSTALL:
                std::cout << suffer::utils::io::cInfo() << suffer::utils::io::yellow(" uninstall\n");
                std::cout << suffer::utils::io::red("\tRemoves a package from the registry.\n\n");
                std::cout << suffer::utils::io::magenta("\tExample usage:\n\t\t");
                std::cout << suffer::utils::io::commandString("suffer uninstall ") << suffer::utils::io::yellow("libname") << suffer::utils::io::cyan(" // Removes the library \"libname\"") << "\n";
                break;
            default:
                std::cerr << suffer::utils::io::error() << " This aint it chief, hit the default case in the help command\n";
                exit(EXIT_FAILURE);
                break;
        }
    } catch (std::exception& e) {
        std::cerr << suffer::utils::io::error() << " Command " << suffer::utils::io::dataString(arguments[0]) << " does not exist\n";
        exit(EXIT_FAILURE);
    }
}