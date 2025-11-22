#pragma once

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <cstdlib>
#include <unistd.h>

#include "../../lib/json.hpp"

#include "./package.hpp"

namespace suffer::core {
    class RegistryHandler {

    private:
        const std::filesystem::path REGISTRY_PATH;
        const std::filesystem::path LIBS_PATH;
        const std::filesystem::path CACHE_PATH;

        //throws an error if failed to create
        void checkCreated(const bool CREATED, const std::filesystem::path& PATH);
        nlohmann::json generateSettings();

    public:
        static const std::filesystem::path getRegistryPath();
        void createRegistry();
        void nukeRegistry();

        const std::vector<suffer::core::Package> getAllPackages();

        const suffer::core::Package findPackage(const std::string& name);
        void deletePackage(suffer::core::Package& oldPackage);
        
        RegistryHandler();
    };
}