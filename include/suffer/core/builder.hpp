#pragma once

#include <iostream>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

#include <nlohmann/json.hpp>

#include <suffer/core/package.hpp>
#include <suffer/core/registryHandler.hpp>
#include <suffer/utils/io.hpp>
#include <suffer/utils/constants.hpp>
#include <suffer/utils/config.hpp>

namespace suffer::core {
    enum HeaderStyle {
        IH_H_STYLE,
        SI_H_STYLE,
        RT_H_STYLE,
        NM_H_STYLE,
        SR_IH_STYLE,
        SN_IH_STYLE,
        SN_IN_STYLE,
        UNKNOWN
    };

    class Builder {
    private:
        suffer::core::Package& package;
        suffer::core::RegistryHandler& registry;

        bool fileIsHeader(const std::filesystem::path& path);
        //returns an enum type, defaults to UNKNOWN
        int determineHeaderPackaging();
        //returns a string of the order in which to link
        std::string determineLinkOrder(const std::filesystem::path& projectPath);
        //called if the header structure is determined to be RT_H_STYLE
        std::vector<std::filesystem::path> getHeadersFromRoot();
        
        //checks for rw, errors out if none
        void checkPermissions(const std::filesystem::path& path);
        
        //finds the compiled .a file inside ~/.suffer/lib - ran after compilation
        std::vector<std::filesystem::path> findStaticLibLocation();

        //copies headers from ~/.suffer/lib
        void importHeaders(const std::filesystem::path& include, const std::filesystem::path& libPath);
        //attempts to detect headers that were built
        std::vector<std::filesystem::path> detectBuiltHeaders();
        //places built headers correctly
        void placeBuiltHeaders(const std::vector<std::filesystem::path>& builtHeaders, const std::filesystem::path& include);
        //attempts to compile a lib
        void compileLib();

        //checks if lib.a exits
        bool isCached();

        //returns a vector with paths to dirs to headers
        std::vector<std::filesystem::path>& subdirsWithHeaders(const std::filesystem::path& rootToSearch, std::vector<std::filesystem::path>& found);
        //determines the apropriate compile command for a project
        std::string determineProjectGpp(const std::filesystem::path& projectPath, const bool mashing = false);
        //dumps json to a file representing dependency order
        void createProjectJson(const int index, const std::vector<std::string>& sysLibs);
        std::filesystem::path determineUnknownHeaderRoot(const std::filesystem::path& libPath);
        void importUnknownHeaders(const std::filesystem::path& include, const std::filesystem::path& libPath);
        

    public:
        Builder(suffer::core::Package& package, suffer::core::RegistryHandler& registry);

        //returns the index which -lLIB needs to be added in suffer.project.json
        int determineLinkingIndex();
        //execute method
        void import(const int index, const bool root = true);
        //sets up a new suffer project
        void setupProject();
        //creates a Makefile with the compile command
        void createMakeFile();
    };
}