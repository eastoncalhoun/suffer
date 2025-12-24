#pragma once

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <optional>

#include <unistd.h>

#include <nlohmann/json.hpp>

#include <suffer/core/package.hpp>
#include <suffer/utils/io.hpp>

namespace suffer::core {
    class RegistryHandler {

    private:
        const std::filesystem::path REGISTRY_PATH;
        const std::filesystem::path LIBS_PATH;
        const std::filesystem::path CACHE_PATH;
        const std::filesystem::path SCRIPTS_PATH;

        void checkCreated(const bool CREATED, const std::filesystem::path& PATH); // throws an error if failed to create at a path
        nlohmann::json generateSettings();

    public:
        static const std::filesystem::path getRegistryPath();
        static const std::filesystem::path getLibsPath();
        static const std::filesystem::path getCachePath();
        static const std::filesystem::path getScriptsPath();

        void createRegistry();
        void nukeRegistry();

        const std::vector<suffer::core::Package> getAllPackages();
        const suffer::core::Package findPackage(const std::string& name);

        void createPackageJson(suffer::core::Package& newPackage);
        void deletePackage(const std::string& oldPackageName);
        
        std::optional<nlohmann::json> knownPackage(const std::string& pName);
        
        RegistryHandler();
    };
}