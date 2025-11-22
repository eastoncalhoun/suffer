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

nlohmann::json suffer::core::RegistryHandler::generateSettings() {
    nlohmann::json jSettings = {
        {"compilerFlags", ""}
    };

    return jSettings;
}

void suffer::core::RegistryHandler::createRegistry() {
    this->checkCreated(std::filesystem::create_directory(this->REGISTRY_PATH), this->REGISTRY_PATH);
    this->checkCreated(std::filesystem::create_directory(this->LIBS_PATH), this->LIBS_PATH);
    this->checkCreated(std::filesystem::create_directory(this->CACHE_PATH), this->CACHE_PATH);

    std::ofstream settingsJson { this->REGISTRY_PATH / "settings.json" };

    if (!settingsJson) {
        std::cerr << "Error: packages.json could not be created\n";
        exit(EXIT_FAILURE);
    }

    settingsJson << this->generateSettings().dump(4);

    settingsJson.close();
}

void suffer::core::RegistryHandler::nukeRegistry() {
    if (!std::filesystem::exists(this->REGISTRY_PATH)) {
        std::cerr << "Registry doesn't exist how did you get here?\n";
        exit(EXIT_FAILURE);
    }

    std::filesystem::remove_all(this->REGISTRY_PATH);
}

const std::vector<suffer::core::Package> suffer::core::RegistryHandler::getAllPackages() {
    std::vector<suffer::core::Package> packages = {};

    for (auto entry : std::filesystem::directory_iterator(this->LIBS_PATH)) {
        std::filesystem::path pPath = entry.path() / "suffer.json";
        
        if (std::filesystem::exists(pPath)) {
            std::ifstream pFile = { pPath };

            if (!pFile) {
                std::cerr << "Error opening " << pPath.c_str() << "\n";
                exit(EXIT_FAILURE);
            }

            std::string fData = std::string(std::istreambuf_iterator<char>(pFile), std::istreambuf_iterator<char>());
            
            pFile.close();

            nlohmann::json pJson = nlohmann::json::parse(fData);

            suffer::core::Package package = suffer::core::Package(
                pJson.value("package", "undefined"),
                pJson.value("version", "undefined"),
                pJson.value("author", "undefined"),
                pJson.value("source", "undefined"),
                pJson.value("headerOnly", false),
                pJson["dependencies"].get<std::map<std::string, std::string>>()
            );

            packages.push_back(package);
        }
    }

    return packages;
}

const suffer::core::Package suffer::core::RegistryHandler::findPackage(const std::string& name) {
    for (auto entry : std::filesystem::directory_iterator(this->LIBS_PATH)) {
        std::filesystem::path pPath = entry.path() / "suffer.json";
        
        if (std::filesystem::exists(pPath)) {
            std::ifstream pFile = { pPath };

            if (!pFile) {
                std::cerr << "Error opening " << pPath.c_str() << "\n";
                exit(EXIT_FAILURE);
            }

            std::string fData = std::string(std::istreambuf_iterator<char>(pFile), std::istreambuf_iterator<char>());
            
            pFile.close();

            nlohmann::json pJson = nlohmann::json::parse(fData);
            std::string pName = pJson.value("package", "undefined");

            if (pName == name) {
                return suffer::core::Package(
                    pJson.value("package", "undefined"),
                    pJson.value("version", "undefined"),
                    pJson.value("author", "undefined"),
                    pJson.value("source", "undefined"),
                    pJson.value("headerOnly", false),
                    pJson["dependencies"].get<std::map<std::string, std::string>>()
                );
            }
        }
    }
}

suffer::core::RegistryHandler::RegistryHandler() : REGISTRY_PATH(this->getRegistryPath()), LIBS_PATH(this->getRegistryPath() / "libs"), CACHE_PATH(this->getRegistryPath() / "cache") {
    if (!std::filesystem::exists(this->REGISTRY_PATH)) {
        std::cout << "Registry not detected. Attempting to generate.\n";
        this->createRegistry();
        std::cout << "Registry generated successfully\n";
    }
}