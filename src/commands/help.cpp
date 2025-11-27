#include "./help.hpp"

void suffer::commands::help(std::vector<std::string>& arguments) {
    if (arguments.size() < 1) {
        std::cout << suffer::utils::io::info() << " Help Command\n";
        std::cout << suffer::utils::io::cInfo() << suffer::utils::io::yellow(" Help\t") << suffer::utils::io::red("Runs this command. Try ") << suffer::utils::io::commandString("suffer help import\n");
        std::cout << suffer::utils::io::cInfo() << suffer::utils::io::yellow(" Install\t") << suffer::utils::io::red("Install a package to the registry.\n");
        std::cout << suffer::utils::io::cInfo() << suffer::utils::io::yellow(" Import\t") << suffer::utils::io::red("Imports a package to the current working directory.\n");
        std::cout << suffer::utils::io::cInfo() << suffer::utils::io::yellow(" List\t") << suffer::utils::io::red("Displays package information.\n");
        std::cout << suffer::utils::io::cInfo() << suffer::utils::io::yellow(" Uninstall\t") << suffer::utils::io::red("Uninstalls a package.\n");
        return;
    }
    
    try {
        switch(SWITCHED[arguments[0]]) {
            case HELP:
                std::cout << suffer::utils::io::cInfo() << suffer::utils::io::yellow(" Help\n");
                std::cout << suffer::utils::io::red("\tRuns this command.\n\n");
                std::cout << suffer::utils::io::magenta("\tExample usage:\n\t\t");
                std::cout << suffer::utils::io::commandString("suffer help import ") << suffer::utils::io::cyan("// Displays information about the import command\n\t\t");
                std::cout << suffer::utils::io::commandString("suffer help ") << suffer::utils::io::cyan("// Displays all commands\n");
                break;
            case INSTALL:
                std::cout << suffer::utils::io::cInfo() << suffer::utils::io::yellow(" Install\n");
                std::cout << suffer::utils::io::red("\tInstalls a package to the repository.\n");
                std::cout << suffer::utils::io::red("\tYou may either install from a github link, or from a local directory.\n\n");
                std::cout << suffer::utils::io::magenta("\tExample usage:\n\t\t");
                std::cout << suffer::utils::io::commandString("suffer install ") << suffer::utils::io::yellow("https://github/author/repository") << suffer::utils::io::cyan(" // Installs from github\n\t\t");
                std::cout << suffer::utils::io::commandString("suffer install --local ") << suffer::utils::io::cyan("// Installs from a local file\n\n");
                std::cout << suffer::utils::io::red("\tNOTE: ") << suffer::utils::io::yellow(" When installing locally, you must install from the root directory of the library you wish to install.\n");
                break;
            case IMPORT:
                std::cout << suffer::utils::io::cInfo() << suffer::utils::io::yellow(" Import\n");
                std::cout << suffer::utils::io::red("\tImports a package to the current working directory.\n\n");
                std::cout << suffer::utils::io::magenta("\tExample usage:\n\t\t");
                std::cout << suffer::utils::io::commandString("suffer import ") << suffer::utils::io::yellow("libname") << suffer::utils::io::cyan(" // Imports the library \"libname\"\n\n");
                std::cout << suffer::utils::io::red("\tNOTE: ") << suffer::utils::io::yellow(" This tool always assumes that any time you are importing, it is to the current working directory.\n");
                break;
            case LIST:
                std::cout << suffer::utils::io::cInfo() << suffer::utils::io::yellow(" List\n");
                std::cout << suffer::utils::io::red("\tLists information about packages.\n\n");
                std::cout << suffer::utils::io::magenta("\tExample usage:\n\t\t");
                std::cout << suffer::utils::io::commandString("suffer list ") << suffer::utils::io::cyan(" // Lists all libraries\n\t\t");
                std::cout << suffer::utils::io::commandString("suffer list ") << suffer::utils::io::yellow("libname") << suffer::utils::io::cyan(" // Lists information about the library \"libname\"\n");
                break;
            case UNINSTALL:
                std::cout << suffer::utils::io::cInfo() << suffer::utils::io::yellow(" Uninstall\n");
                std::cout << suffer::utils::io::red("\tRemoves a package from the registry.\n\n");
                std::cout << suffer::utils::io::magenta("\tExample usage:\n\t\t");
                std::cout << suffer::utils::io::commandString("suffer uninstall ") << suffer::utils::io::yellow("libname") << suffer::utils::io::cyan(" // Removes the library \"libname\"\n\n");
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