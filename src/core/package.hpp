#pragma once

#include <string>
#include <map>
#include <filesystem>

#include <unistd.h>

#include "../../include/nlohmann/json.hpp"

namespace suffer::core {
    
    class Package {
    private:
        std::string name;
        std::string version;
        std::string author;
        std::string source;

        bool headerOnly;

        std::map<std::string, std::string> dependencies;

    public:
        Package(const std::string& name, const std::string& version, const std::string& author, const std::string& source, const bool headerOnly, const std::map<std::string, std::string>& dependencies);

        const std::string& getName();
        const std::string& getVersion();
        const std::string& getAuthor();
        const std::string& getSource();
        const std::map<std::string, std::string>& getDependencies();

        const bool isHeaderOnly();
        const bool isNull();

        const std::string toJsonText();

        static Package nullPackage();
        //The package's path. Probably.
        const std::filesystem::path determinePath();
    };
}