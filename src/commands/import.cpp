#include "./import.hpp"

void suffer::commands::import(std::vector<std::string>& args) {
    if (args.size() < 1) {
        std::cerr << suffer::utils::io::error() << " Not enough arguments supplied to " << suffer::utils::io::dataString("import") << "\n";
        std::cout << suffer::utils::io::info() << " Try " << suffer::utils::io::commandString("suffer help") << "\n";
        exit(EXIT_FAILURE);
    }

    std::cout << suffer::utils::io::info() << " Currently, only header libs are supported\n";

    suffer::core::RegistryHandler registry = suffer::core::RegistryHandler();
    suffer::core::Package package = registry.findPackage(args[0]);
    suffer::core::Builder builder = suffer::core::Builder(package, registry);

    if (!package.isHeaderOnly()) {
        std::cerr << suffer::utils::io::error() << " Non header only lib detected :(\n";
        exit(EXIT_FAILURE);
    }

    builder.import();    
}