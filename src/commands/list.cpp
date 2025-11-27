#include "./list.hpp"

void suffer::commands::list(std::vector<std::string>& arguments) {
    suffer::core::RegistryHandler registry = suffer::core::RegistryHandler();
    
    if (arguments.size() < 1) {
        std::vector<suffer::core::Package> packages = registry.getAllPackages(); 

        for (auto& package : packages) {
            std::cout << suffer::utils::io::pInfo() << " " << suffer::utils::io::dataString(package.getName()) << "\n"; 
        }

        std::cout << suffer::utils::io::okay() << " " << packages.size() << " packages found\n";
    } else {
        suffer::core::Package package = registry.findPackage(arguments[0]);
        std::string hOnly = "false";

        if (package.isHeaderOnly()) {
            hOnly = "true";
        }

        std::cout << suffer::utils::io::pInfo() << " " << suffer::utils::io::dataString(package.getName()) << "\n";
        std::cout << suffer::utils::io::yellow("\tAuthor: ") << suffer::utils::io::dataString(package.getAuthor()) << "\n";
        std::cout << suffer::utils::io::yellow("\tVersion: ") << suffer::utils::io::dataString(package.getVersion()) << "\n";
        std::cout << suffer::utils::io::yellow("\tSource: ") << suffer::utils::io::dataString(package.getSource()) << "\n";
        std::cout << suffer::utils::io::yellow("\tHeader Only: ") << suffer::utils::io::dataString(hOnly) << "\n";

        for (auto& dep : package.getDependencies()) {
            std::cout << suffer::utils::io::yellow("\tDependency: ") << suffer::utils::io::dataString(dep.first + "^" + dep.second) << "\n";
        }
    }
}