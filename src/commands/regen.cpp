#include "./regen.hpp"

void suffer::commands::regen() {
    suffer::core::RegistryHandler registry {};
    suffer::core::Package nullP = suffer::core::Package::nullPackage();
    suffer::core::Builder builder {nullP, registry};

    if (!std::filesystem::exists(std::filesystem::current_path() / "suffer.project.json")) {
        std::cerr << "No suffer.project.json file exists here\n";
        exit(EXIT_FAILURE);
    }

    builder.createMakeFile();
    std::cout << suffer::utils::io::okay() << " Makefile regenerated\n";
}