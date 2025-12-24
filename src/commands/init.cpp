#include <suffer/commands/init.hpp>

void suffer::commands::init() {
    std::cout << suffer::utils::io::info() << " Creating empty project\n";
    
    suffer::core::RegistryHandler registry = suffer::core::RegistryHandler();
    suffer::core::Package nullPackage = suffer::core::Package::nullPackage();
    suffer::core::Builder builder = suffer::core::Builder(nullPackage, registry);

    builder.setupProject();
}