//if empty install install nothing a few times then pretend to rm -rf /
#include "./install.hpp"

void suffer::commands::install(std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cerr << suffer::utils::io::error() << " Not enough arguments supplied to " << suffer::utils::io::dataString("install") << "\n";
        std::cout << suffer::utils::io::info() << " Try " << suffer::utils::io::commandString("suffer help\n");
        exit(EXIT_FAILURE);
    }

    suffer::core::RegistryHandler registryHandler = suffer::core::RegistryHandler();

    std::string input, dualieInput;

    if (args[1] == "--local") {
        std::cout << suffer::utils::io::info() << " You must be in the root directory of your new package, and that directory should be whatever the name of your package is.\n" << suffer::utils::io::red("Are these conditions met? ") << suffer::utils::io::noYes();

        std::cin >> input;

        if (input == "n" || input == "no") {
            std::cout << suffer::utils::io::okay() << " Did nothing successfully!\n";
            return;
        }

        std::string name = std::filesystem::current_path().filename().string();

        if (std::filesystem::exists(registryHandler.getLibsPath() / name)) {
            std::cout << suffer::utils::io::info() << " Package " << suffer::utils::io::dataString(name) << " is already installed\nWould you like to reinstall?" << suffer::utils::io::yesNo();

            std::cin >> input;

            if (input == "n" || input == "no") {
                std::cout << suffer::utils::io::okay() << " Package already exists, exiting\n";
                return;
            } else {
                registryHandler.deletePackage(name);
            }
        }

        if (std::filesystem::exists(std::filesystem::current_path() / "suffer.json")) {
            std::cout << suffer::utils::io::okay() << " suffer.json found\n";

            try {
                std::filesystem::copy(std::filesystem::current_path(), registryHandler.getLibsPath() / name, std::filesystem::copy_options::recursive);
                std::cout << suffer::utils::io::okay() << " Successfully installed " << suffer::utils::io::dataString(name) << "\n";
                return;
            } catch (std::exception& e) {
                std::cerr << suffer::utils::io::error() << " Failed to copy " << suffer::utils::io::dataString(std::filesystem::current_path().string()) << " to " << suffer::utils::io::dataString(registryHandler.getLibsPath().string()) << "\n";
                exit(EXIT_FAILURE);
            }
        }


        std::string version, author;
        bool headerOnly = true;
        std::map<std::string, std::string> dependencies = std::map<std::string, std::string>();

        std::cout << "Package version: ";
        std::cin >> version;

        std::cout << "Package author: ";
        std::cin >> author;

        std::cout << "Is this library only headers? " << suffer::utils::io::noYes();
        std::cin >> input;

        if (input == "n" || input == "no") {
            headerOnly = false;
        }

        std::cout << "Are there any dependencies? " << suffer::utils::io::yesNo();
        std::cin >> input;

        if (input == "y" || input == "yes") {
            while (true) {
                std::cout << "Dependency name: ";
                std::cin >> input;
                std::cout << "Dependency version: ";
                std::cin >> dualieInput;

                dependencies[input] = dualieInput;

                std::cout << "More dependencies? " << suffer::utils::io::yesNo();
                std::cin >> input;

                if (input == "y" || input == "yes") {
                    continue;
                } else {
                    break;
                }
            }
        }

        suffer::core::Package package = suffer::core::Package(name, version, author, "local", headerOnly, dependencies);

        std::cout << suffer::utils::io::info() << " Does this look correct?\n";
        std::cout << package.toJsonText() << suffer::utils::io::noYes();
        std::cin >> input;

        if (input == "n" || input == "no") {
            std::cout << suffer::utils::io::okay() << " Data was incorrect, cancelling install\n";
            return;
        }

        try {
            std::filesystem::copy(std::filesystem::current_path(), registryHandler.getLibsPath() / name, std::filesystem::copy_options::recursive);
        } catch (std::exception& e) {
            std::cerr << suffer::utils::io::error() << " Failed to copy " << suffer::utils::io::dataString(std::filesystem::current_path().string()) << " to " << suffer::utils::io::dataString(registryHandler.getLibsPath().string()) << "\n";
            exit(EXIT_FAILURE);
        }
        
        registryHandler.createPackageJson(package);
        
        std::cout << suffer::utils::io::okay() << " Successfully installed " << suffer::utils::io::dataString(name) << "\n";
    }
}