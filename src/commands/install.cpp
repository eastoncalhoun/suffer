#include "./install.hpp"

void suffer::commands::install(std::vector<std::string>& args) {
    if (args.size() < 1) {
        std::cerr << suffer::utils::io::error() << " Not enough arguments supplied to " << suffer::utils::io::dataString("install") << "\n";
        exit(EXIT_FAILURE);
    }

    const bool local = args[0] == "--local";
    suffer::core::RegistryHandler registryHandler = suffer::core::RegistryHandler();
    const std::filesystem::path start = std::filesystem::current_path();
    std::map<std::string, std::string> dependencies = std::map<std::string, std::string>();
    bool headerOnly = true;
    std::string input, dualieInput, version, author, name, source;
    

    if (local) {
        std::cout << suffer::utils::io::info() << " You must be in the root directory of your new package, and that directory should be whatever the name of your package is.\n" << suffer::utils::io::red("Are these conditions met? ") << suffer::utils::io::noYes();

        std::cin >> input;

        if (input == "n" || input == "no") {
            std::cout << suffer::utils::io::okay() << " Did nothing successfully!\n";
            return;
        }

        name = std::filesystem::current_path().filename().string();
        source = "local";

    } else {
        const std::filesystem::path& reg = registryHandler.getRegistryPath();
        const std::filesystem::path tmp = reg / "tmp";
        const std::string& arg = args[0];
        const std::string gh = "https://github.com/";

        if (arg.find(gh) == std::string::npos) {
            std::cerr << suffer::utils::io::error() << " Invalid url supplied to install\n Only https github and local are currently supported.\n";
            exit(EXIT_FAILURE);
        }

        name = arg.substr(arg.find("m/"));
        name = name.replace(0, name.find("/") + 1, "");
        author = name.substr(0);
        author = author.replace(author.find("/"), author.size(), "");
        name = name.substr(name.find("/") + 1);

        source = arg;

        if (std::filesystem::exists(tmp)) {
            std::filesystem::remove_all(tmp);
        }

        std::filesystem::create_directory(tmp);
        std::filesystem::current_path(tmp);
        std::string command = "git clone " + arg;

        //git clone <url>
        if (system(command.c_str()) != 0) {
            std::cerr << suffer::utils::io::error() << " Failed to clone " << suffer::utils::io::dataString(arg) << "\n";
            exit(EXIT_FAILURE);
        }

        std::cout << (tmp / name).string() << "\n";

        std::filesystem::current_path(tmp / name);
    }

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
    
    if (local) {
        std::cout << "Package author: ";
        std::cin >> author;
    } 

    std::cout << "Package version: ";
    std::cin >> version;

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

    suffer::core::Package package = suffer::core::Package(name, version, author, source, headerOnly, dependencies);

    std::cout << suffer::utils::io::info() << " Does this look correct?\n";
    std::cout << package.toJsonText() << suffer::utils::io::noYes();
    std::cin >> input;

    if (input == "n" || input == "no") {
        std::cout << suffer::utils::io::okay() << " Data was incorrect, cancelling install\n";
        return;
    }

    try {
        std::filesystem::copy(std::filesystem::current_path(), registryHandler.getLibsPath() / name, std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing);
    } catch (std::exception& e) {
        std::cerr << suffer::utils::io::error() << " Failed to copy " << suffer::utils::io::dataString(std::filesystem::current_path().string()) << " to " << suffer::utils::io::dataString(registryHandler.getLibsPath().string()) << "\n";
        exit(EXIT_FAILURE);
    }
    
    registryHandler.createPackageJson(package);

    if (!local) {
        std::filesystem::current_path(start);
        std::filesystem::remove_all(registryHandler.getRegistryPath() / "tmp");
    }
    
    std::cout << suffer::utils::io::okay() << " Successfully installed " << suffer::utils::io::dataString(name) << "\n";
    
}