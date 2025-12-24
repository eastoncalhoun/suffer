#include <suffer/core/package.hpp>

suffer::core::Package::Package(const std::string& name, const std::string& version, const std::string& author, const std::string& source, const bool headerOnly, const std::string& flags, const std::unordered_map<std::string, std::string>& dependencies) {
    this->name = name;
    this->version = version;
    this->author = author;
    this->source = source;
    this->headerOnly = headerOnly;
    this->dependencies = dependencies;
    this->flags = flags;
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

const std::string& suffer::core::Package::getFlags() {
    return this->flags;
}

const std::unordered_map<std::string, std::string>& suffer::core::Package::getDependencies() {
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

    for (std::unordered_map<std::string, std::string>::iterator element = this->dependencies.begin(); element != this->dependencies.end(); element++) {
        keys.push_back(element->first);
    }
    
    nlohmann::json pJ = {
        {"package", this->name},
        {"version", this->version},
        {"author", this->author},
        {"source", this->source},
        {"headerOnly", this->headerOnly},
        {"flags", this->flags}
    };

    pJ["dependencies"] = {};

    for (int i = 0; i < keys.size(); i++) {
        pJ["dependencies"][keys.at(i)] = this->dependencies[keys.at(i)];
    }

    return pJ.dump(4);
}

suffer::core::Package suffer::core::Package::nullPackage() {
    return suffer::core::Package("null", "null", "null", "null", false, "null", std::unordered_map<std::string, std::string>());
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
    const std::string home = getenv("HOME");
    const std::filesystem::path cachePath = std::filesystem::path(home) / ".suffer" / "cache" / this->name;

    return cachePath;
}

suffer::core::Package suffer::core::Package::pathFactory(std::filesystem::path path) {
    std::ifstream pFile(path);

    if (!pFile) {
        std::cerr << suffer::utils::io::error() <<  " Failed to open the file " << suffer::utils::io::dataString(path.string()) << "\n";
        exit(EXIT_FAILURE);
    }

    std::string fData = std::string(std::istreambuf_iterator<char>(pFile),std::istreambuf_iterator<char>());
    
    pFile.close();

    nlohmann::json pJson = nlohmann::json(); 
    std::string pName;
    std::unordered_map<std::string, std::string> deps;

    try {
        pJson = nlohmann::json::parse(fData);
    } catch (std::exception& e) {
        std::cerr << suffer::utils::io::error() << " Invalid json at " << suffer::utils::io::dataString(path.string()) << "\n";
        exit(EXIT_FAILURE);
    }

    try {
        deps = pJson["dependencies"].get<std::unordered_map<std::string, std::string>>();
    } catch (std::exception& e) {
        deps = std::unordered_map<std::string, std::string>();
    }

    try {
        return suffer::core::Package(
            pJson["package"].get<std::string>(),
            pJson["version"].get<std::string>(),
            pJson["author"].get<std::string>(),
            pJson["source"].get<std::string>(),
            pJson["headerOnly"].get<bool>(),
            pJson["flags"].get<std::string>(),
            deps
        );
    } catch (std::exception& e) {
        std::cout << suffer::utils::io::warning() << " Invalid json at " << suffer::utils::io::dataString(path.string()) << "\n" << e.what() << "\n";

        return suffer::core::Package::nullPackage();
    }
}