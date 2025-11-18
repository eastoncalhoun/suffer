#include "./registryHandler.hpp"

void suffer::core::RegistryHandler::checkCreated(const bool CREATED, const std::filesystem::path& PATH) {
    if (!CREATED) {
        std::cerr << "Failed to create " << PATH << std::endl;
        exit(EXIT_FAILURE);
    }
}

const std::filesystem::path suffer::core::RegistryHandler::getRegistryPath() {
    if (geteuid() == 0) {
        std::cerr << "NO!\nABSOLUTELY NOT!!\nDon't run this as root!!!\nI'm a random dude on the internet!!!!\n>:(!!!!!!!!\n";
        exit(EXIT_FAILURE);
    }

    const char* HOME = std::getenv("HOME");

    if (!HOME || HOME[0] == '\0') {
        std::cerr << "$HOME is not set. It's freezing outside.\n";
        sleep(1);
        std::cout << "Let " << std::flush;
        sleep(1);
        std::cout << "me " << std::flush;
        sleep(1);
        std::cout << "in.\n";

        exit(EXIT_FAILURE);
    }

    return std::filesystem::path(HOME) / ".suffer";
}

suffer::core::RegistryHandler::RegistryHandler() : REGISTRY_PATH(this->getRegistryPath()), LIBS_PATH(this->getRegistryPath() / "libs"), CACHE_PATH(this->getRegistryPath() / "cache") {
    if (!std::filesystem::exists(this->REGISTRY_PATH)) {
        this->checkCreated(std::filesystem::create_directory(this->REGISTRY_PATH), this->REGISTRY_PATH);
        this->checkCreated(std::filesystem::create_directory(this->LIBS_PATH), this->LIBS_PATH);
        this->checkCreated(std::filesystem::create_directory(this->CACHE_PATH), this->CACHE_PATH);
    }
}