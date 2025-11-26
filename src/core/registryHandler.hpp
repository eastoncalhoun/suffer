#pragma once

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>

#include <unistd.h>

#include "../../include/nlohmann/json.hpp"

#include "./package.hpp"
#include "../utils/io.hpp"

namespace suffer::core {
    class RegistryHandler {

    private:
        const std::filesystem::path REGISTRY_PATH;
        const std::filesystem::path LIBS_PATH;
        const std::filesystem::path CACHE_PATH;

        //throws an error if failed to create at a path
        void checkCreated(const bool CREATED, const std::filesystem::path& PATH);
        nlohmann::json generateSettings();        

    public:
        static const std::filesystem::path getRegistryPath();
        
        const std::filesystem::path getLibsPath();
        const std::filesystem::path getCachePath();

        void createRegistry();
        void createPackageJson(suffer::core::Package& newPackage);

        void nukeRegistry();

        const std::vector<suffer::core::Package> getAllPackages();
        const suffer::core::Package findPackage(const std::string& name);

        void deletePackage(const std::string& oldPackageName);        
        
        RegistryHandler();
    };
}