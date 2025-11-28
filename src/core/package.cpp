#include "./package.hpp"

suffer::core::Package::Package(const std::string& name, const std::string& version, const std::string& author, const std::string& source, const bool headerOnly, const std::map<std::string, std::string>& dependencies) {
    this->name = name;
    this->version = version;
    this->author = author;
    this->source = source;
    this->headerOnly = headerOnly;
    this->dependencies = dependencies;
}

const std::string& suffer::core::Package::getName() {
    return this->name;
}

const std::string& suffer::core::Package::getVersion() {
    return this->version;
}

const std::string& suffer::core::Package::getAuthor() {
    return this->author;
}

const std::string& suffer::core::Package::getSource() {
    return this->source;
}

const std::map<std::string, std::string>& suffer::core::Package::getDependencies() {
    return this->dependencies;
}

const bool suffer::core::Package::isHeaderOnly() {
    return this->headerOnly;
}

const bool suffer::core::Package::isNull() {
    suffer::core::Package null = suffer::core::Package::nullPackage();

    return this->name == null.getName() && this->version == null.getVersion() && this->author == null.getAuthor() && this->source == null.getSource();
}

const std::string suffer::core::Package::toJsonText() {
    std::vector<std::string> keys = {};

    for (std::map<std::string, std::string>::iterator element = this->dependencies.begin(); element != this->dependencies.end(); element++) {
        keys.push_back(element->first);
    }
    
    nlohmann::json pJ = {
        {"package", this->name},
        {"version", this->version},
        {"author", this->author},
        {"source", this->source},
        {"headerOnly", this->headerOnly}
    };

    pJ["dependencies"] = {};

    for (int i = 0; i < keys.size(); i++) {
        pJ["dependencies"][keys.at(i)] = this->dependencies[keys.at(i)];
    }

    return pJ.dump(4);
}

suffer::core::Package suffer::core::Package::nullPackage() {
    return suffer::core::Package("null", "null", "null", "null", false, std::map<std::string, std::string>());
}

const std::filesystem::path suffer::core::Package::determinePath() {
    const std::string home = getenv("HOME");
    const std::filesystem::path libPath = std::filesystem::path(home) / ".suffer" / "libs" / this->name;

    if (!std::filesystem::exists(libPath)) {
        std::cout << suffer::utils::io::warning() << " The path " << suffer::utils::io::dataString(libPath) << " does not exist (package::determinePath)\n";
    }

    return libPath;
}

const std::filesystem::path suffer::core::Package::determineCachePath() {
    const std::string fileName = "lib" + this->name + ".a";
    const std::string home = getenv("HOME");
    const std::filesystem::path cachePath = std::filesystem::path(home) / ".suffer" / "cache" / fileName;

    if (!std::filesystem::exists(cachePath)) {
        std::cout << suffer::utils::io::warning() << " The path " << suffer::utils::io::dataString(cachePath) << " does not exist (package::determineCachePath)\n";
    }

    return cachePath;
}