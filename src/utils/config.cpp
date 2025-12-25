#include <suffer/utils/config.hpp>

nlohmann::json suffer::utils::Config::toJson() {
    nlohmann::json json {  };

    json["compilerCpp"] = this->compilerCpp;
    json["compilerC"] = this->compilerC;
    json["stdCpp"] = this->stdCpp;
    json["stdC"] = this->stdC;
    json["compilerFlags"] = this->compilerFlags;
    json["cmakeFlags"] = this->cmakeFlags;

    return json;
}

void suffer::utils::Config::writeConfig() {
    std::ofstream fConfig { this->configPath };

    if (!fConfig) {
        std::cerr << suffer::utils::io::error() << " Failed to open " << suffer::utils::io::dataString(this->configPath) << "\n";
        exit(EXIT_FAILURE);
    }

    fConfig << this->toJson().dump(4);
}

nlohmann::json suffer::utils::Config::rawConfig() {
    std::ifstream fConfig { this->configPath };

    if (!fConfig) {
        std::cerr << suffer::utils::io::error() << " Failed to open " << suffer::utils::io::dataString(this->configPath) << "\n";
        exit(EXIT_FAILURE);
    }

    const std::string  sConfig = std::string(std::istreambuf_iterator<char>(fConfig), std::istreambuf_iterator<char>());
    fConfig.close();

    nlohmann::json jConfig;

    try {
        jConfig = nlohmann::json::parse(sConfig);
    } catch (std::exception& e) {
        std::cout << suffer::utils::io::error() << " Suffer's configuration file " << suffer::utils::io::dataString(this->configPath) << " contained invalid json\n";
        exit(EXIT_FAILURE);
    }

    return jConfig;
}

const std::filesystem::path suffer::utils::Config::getPath() {
    return suffer::core::RegistryHandler::getRegistryPath() / "config.json";
}

void suffer::utils::Config::setCompilerCpp(const std::string& compiler) {
    this->compilerCpp = compiler;
    this->writeConfig();
}

void suffer::utils::Config::setCompilerC(const std::string& compiler) {
    this->compilerC = compiler;
    this->writeConfig();
}

void suffer::utils::Config::setStdCpp(const std::string& standard) {
    this->stdCpp = standard;
    this->writeConfig();
}

void suffer::utils::Config::setStdC(const std::string& standard) {
    this->stdC = standard;
    this->writeConfig();
}

void suffer::utils::Config::setCompilerFlags(const std::string& flags) {
    this->compilerFlags = flags;
    this->writeConfig();
}

void suffer::utils::Config::setCmakeFlags(const std::string& flags) {
    this->compilerCpp = flags;
    this->writeConfig();
}

const std::string& suffer::utils::Config::getCompilerCpp() {
    return this->compilerCpp;
}

const std::string& suffer::utils::Config::getCompilerC() {
    return this->compilerC;
}

const std::string& suffer::utils::Config::getStdCpp() {
    return this->stdCpp;
}

const std::string& suffer::utils::Config::getStdC() {
    return this->stdC;
}

const std::string& suffer::utils::Config::getCompilerFlags() {
    return this->compilerFlags;
}

const std::string& suffer::utils::Config::getCmakeFlags() {
    return this->cmakeFlags;
}

suffer::utils::Config::Config() {
    this->configPath = this->getPath();

    if (!std::filesystem::exists(this->configPath)) {
        std::cerr << suffer::utils::io::error() << " Suffer's configuration file " << suffer::utils::io::dataString(this->configPath) << " does not exist\n";
        exit(EXIT_FAILURE);
    }

    nlohmann::json raw = this->rawConfig();

    try {
        this->compilerCpp = raw["compilerCpp"].get<std::string>();
        this->compilerC = raw["compilerC"].get<std::string>();
        this->stdCpp = raw["stdCpp"].get<std::string>();
        this->stdC = raw["stdC"].get<std::string>();
        this->compilerFlags = raw["compilerFlags"].get<std::string>();
        this->cmakeFlags = raw["cmakeFlags"].get<std::string>();
    } catch (std::exception& e) {
        std::cerr << suffer::utils::io::error() << " Suffer's configuration file " << suffer::utils::io::dataString(this->configPath) << " did not contain the required data\n";
        exit(EXIT_FAILURE);
    }
}
