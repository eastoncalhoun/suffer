#include "./registryHandler.hpp"

const std::filesystem::path suffer::core::RegistryHandler::getLibsPath() {
    return this->LIBS_PATH;
}

const std::filesystem::path suffer::core::RegistryHandler::getCachePath() {
    return this->CACHE_PATH;
}

void suffer::core::RegistryHandler::checkCreated(const bool CREATED, const std::filesystem::path& PATH) {
    if (!CREATED) {
        std::cerr << suffer::utils::io::error() << " Failed to create " << suffer::utils::io::dataString(PATH.string()) << "\n";
        exit(EXIT_FAILURE);
    }
}

const std::filesystem::path suffer::core::RegistryHandler::getRegistryPath() {
    const char* HOME = std::getenv("HOME");

    if (!HOME || HOME[0] == '\0') {
        std::cerr <<  suffer::utils::io::error() << " The " << suffer::utils::io::blue("$HOME ") << " environment variable is not set\n";

        std::cout << "Try " << suffer::utils::io::yellow("$") << suffer::utils::io::blue("export HOME=") << suffer::utils::io::yellow("/path/to/your/home/directory") << " ?\n";

        sleep(1);
        std::cout << suffer::utils::io::blue("It's freezing outside.. ") << std::flush;
        sleep(1);
        std::cout << suffer::utils::io::red("Let ") << std::flush;
        sleep(1);
        std::cout << suffer::utils::io::red("me ") << std::flush;
        sleep(1);
        std::cout << suffer::utils::io::red("in.\n");

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
        std::cerr << suffer::utils::io::error() << " The " << suffer::utils::io::dataString((this->REGISTRY_PATH / "settings.json").string()) << " file could not be created.\n";
        exit(EXIT_FAILURE);
    }

    settingsJson << this->generateSettings().dump(4);

    settingsJson.close();

    std::cout << suffer::utils::io::okay() << " The registry has been generated.\n";
}

void suffer::core::RegistryHandler::nukeRegistry() {
    if (!std::filesystem::exists(this->REGISTRY_PATH)) {
        std::cerr << suffer::utils::io::error() << " The registry that was to be destroyed doesn't exist. How did you get here?\n";
        exit(EXIT_FAILURE);
    }

    std::filesystem::remove_all(this->REGISTRY_PATH);
}

const std::vector<suffer::core::Package> suffer::core::RegistryHandler::getAllPackages() {
    std::vector<suffer::core::Package> packages = {};

    for (auto entry : std::filesystem::directory_iterator(this->LIBS_PATH)) {
        packages.push_back(suffer::core::Package::pathFactory(entry.path() / "suffer.json"));
    }

    return packages;
}

const suffer::core::Package suffer::core::RegistryHandler::findPackage(const std::string& name) {
    for (auto entry : std::filesystem::directory_iterator(this->LIBS_PATH)) {
        std::filesystem::path pPath = entry.path() / "suffer.json";
        
        if (!std::filesystem::exists(pPath)) {
            std::cout << suffer::utils::io::warning() << " No suffer.json file found at " + suffer::utils::io::dataString(pPath.string()) +"\nIf this is a package, you must create a suffer.json file at " << suffer::utils::io::dataString(entry.path().string()) << "\n";
        }

        if (entry.path().filename().string().find(name) == std::string::npos || entry.path().filename().string().size() != name.size()) {
            continue;
        } 

        return suffer::core::Package::pathFactory(pPath);        
    }

    std::cerr << suffer::utils::io::error() << " The package " << suffer::utils::io::dataString(name) << " was not found in the registry\n";
    exit(EXIT_FAILURE);
}

void suffer::core::RegistryHandler::deletePackage(const std::string& oldPackageName) {
    Package p = this->findPackage(oldPackageName);

    std::filesystem::remove_all(this->LIBS_PATH / oldPackageName);
}

void suffer::core::RegistryHandler::createPackageJson(suffer::core::Package& newPackage) {
    const std::filesystem::path pPath = this->LIBS_PATH / newPackage.getName();
    const std::filesystem::path pJPath = pPath / "suffer.json";

    if (newPackage.isNull()) {
        std::cerr << suffer::utils::io::error() << " The proposed new package is a null package\n";
        exit(EXIT_FAILURE);
    }

    if (!std::filesystem::exists(pPath)) {
        std::cerr << suffer::utils::io::error() << " There is nothing at " << suffer::utils::io::dataString(pPath) << " to generate for\n";
        exit(EXIT_FAILURE);
    }

    if (std::filesystem::exists(pJPath)) {
        std::string input;

        std::cout << suffer::utils::io::info() << " The suffer.json file already exists in " << suffer::utils::io::dataString(pPath.string()) << "\nWould you like to make a new one?\n" << suffer::utils::io::yesNo();

        std::cin >> input;

        if (input != "y" && input != "yes") {
            return;
        }
    }

    std::ofstream jFile { pJPath };
    
    if (!jFile) {
        std::cerr << suffer::utils::io::error() << " Failed to open " << suffer::utils::io::dataString(pJPath.string()) << "\n";
        exit(EXIT_FAILURE);
    }

    jFile << newPackage.toJsonText();
    jFile.close();

    std::cout << suffer::utils::io::okay() << " Successfully created " << suffer::utils::io::dataString(pJPath.string()) << "\n";
}

std::optional<nlohmann::json> suffer::core::RegistryHandler::knownPackage(const std::string& pName) {
    const std::filesystem::path knownPath = this->REGISTRY_PATH / "known.json";
    std::optional<nlohmann::json> known;
    std::ifstream knownFile(knownPath);
    nlohmann::json knownJson;

    if (!knownFile) {
        std::cerr << suffer::utils::io::error() << " Failed to open " << suffer::utils::io::dataString(knownPath.string());
        knownFile.close();
        exit(EXIT_FAILURE);
    }

    const std::string fContents = std::string(std::istreambuf_iterator<char>(knownFile), std::istreambuf_iterator<char>());

    try {
        knownJson = nlohmann::json::parse(fContents);
    } catch (std::exception& e) {
        std::cerr << suffer::utils::io::error() << " Invalid json at " << suffer::utils::io::dataString(knownPath.string());
        exit(EXIT_FAILURE);
    }

    try {
        known = knownJson[pName].get<nlohmann::json>();
        knownJson[pName]["headerOnly"].get<bool>(); // gotta try and access a property or it is null.
    } catch (std::exception& e) {
        known = std::nullopt;
    }

    return known;
}

suffer::core::RegistryHandler::RegistryHandler() : REGISTRY_PATH(this->getRegistryPath()), LIBS_PATH(this->getRegistryPath() / "libs"), CACHE_PATH(this->getRegistryPath() / "cache") {
    if (!std::filesystem::exists(this->REGISTRY_PATH) || !std::filesystem::exists(this->CACHE_PATH) || !std::filesystem::exists(this->LIBS_PATH)) {
        std::string input;

        std::cout << suffer::utils::io::error() << " No registry was detected at " << suffer::utils::io::dataString(this->REGISTRY_PATH.string()) << ".\nWould you like to try and generate one?" << suffer::utils::io::yesNo();

        std::cin >> input;

        if (input == "y" || input == "yes") {
            this->createRegistry();
        }
    }
}