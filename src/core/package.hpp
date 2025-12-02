#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <fstream>

#include <unistd.h>

#include "../../include/nlohmann/json.hpp"

#include "../utils/io.hpp"

namespace suffer::core {
    
    class Package {
    private:
        std::string name;
        std::string version;
        std::string author;
        std::string source;
        std::string flags;

        bool headerOnly;

        std::unordered_map<std::string, std::string> dependencies;

    public:
        Package(const std::string& name, const std::string& version, const std::string& author, const std::string& source, const bool headerOnly, const std::string& flags, const std::unordered_map<std::string, std::string>& dependencies);
        
        static Package pathFactory(std::filesystem::path path);

        const std::string& getName();
        const std::string& getVersion();
        const std::string& getAuthor();
        const std::string& getSource();
        const std::string& getFlags();
        const std::unordered_map<std::string, std::string>& getDependencies();

        const bool isHeaderOnly();
        const bool isNull();

        const std::string toJsonText();

        const static bool isKnown(const std::string& name);
        static Package nullPackage();
        //The package's path. Probably.
        const std::filesystem::path determinePath();
        //The package's .a path. Probably..
        const std::filesystem::path determineCachePath();
    };
}