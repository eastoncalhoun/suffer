#pragma once

#include <iostream>
#include <filesystem>
#include <string>
#include <map>

#include "./package.hpp"
#include "./registryHandler.hpp"
#include "../utils/io.hpp"

namespace suffer::core {
    enum {
        IH_H_HEADER_STYLE,
        SI_H_HEADER_STYLE,
        RT_H_STYLE,
        UNKNOWN
    };

    class Builder {
    private:
        suffer::core::Package& package;
        suffer::core::RegistryHandler& registry;
        
        void checkPermissions(const std::filesystem::path& path);
        void prevImportDetected();
        int determineHeaderPackaging();
        std::vector<std::filesystem::path> getHeadersFromRoot();
        std::filesystem::path determineStaticLibLocation();
        std::string determineCompileCommand();
        std::string determineMakeFile();

        void importHeaders(const std::filesystem::path& include, const std::filesystem::path& libPath);

    public:
        Builder(suffer::core::Package& package, suffer::core::RegistryHandler& registry);

        void import();
    };
}