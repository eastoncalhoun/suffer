#include <suffer/commands/clean.hpp>

void suffer::commands::clean(std::vector<std::string>& arguments) {
    suffer::core::RegistryHandler registry = suffer::core::RegistryHandler();
    std::filesystem::path cache = registry.getCachePath();
    std::filesystem::remove_all(cache);
    std::filesystem::create_directory(cache);

    std::cout << suffer::utils::io::okay() << " Deleted all compiled libs in " << suffer::utils::io::dataString(cache) << "\n";
    std::cout << suffer::utils::io::info() << " You must re-import to your project for rebuilt packages\n";
}