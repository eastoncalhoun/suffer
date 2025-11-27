#include "./uninstall.hpp"

void suffer::commands::uninstall(std::vector<std::string>& arguments) {
    if (arguments.size() < 1) {
        std::cerr << suffer::utils::io::error() << " Not enough arguments supplied to uninstall\n";
        exit(EXIT_FAILURE);
    }

    std::cout << suffer::utils::io::info() << " Attempting to uninstall " << suffer::utils::io::dataString(arguments[0]) << "\n";

    suffer::core::RegistryHandler registry = suffer::core::RegistryHandler();
    
    registry.deletePackage(arguments[0]);
    
    std::cout << suffer::utils::io::okay() << " Successfully uninstalled " << suffer::utils::io::dataString(arguments[0]) << "\n";
}