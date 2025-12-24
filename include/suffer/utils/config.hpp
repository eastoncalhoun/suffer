#pragma once

#include <filesystem>
#include <fstream>
#include <string>

#include <nlohmann/json.hpp>

#include <suffer/core/registryHandler.hpp>
#include <suffer/utils/io.hpp>

namespace suffer::utils {
    class Config {
        private:

        std::filesystem::path configPath;
        std::string compilerCpp;
        std::string compilerC;
        std::string stdCpp;
        std::string stdC;
        std::string compilerFlags;
        std::string cmakeFlags;

        nlohmann::json toJson();
        void writeConfig();
        nlohmann::json rawConfig();

        public:

        static const std::filesystem::path getPath();

        void setCompilerCpp(const std::string& compiler);
        void setCompilerC(const std::string& compiler);
        void setStdCpp(const std::string& standard);
        void setStdC(const std::string& standard);
        void setCompilerFlags(const std::string& flags);
        void setCmakeFlags(const std::string& flags);
        
        const std::string& getCompilerCpp();
        const std::string& getCompilerC();
        const std::string& getStdCpp();
        const std::string& getStdC();
        const std::string& getCompilerFlags();
        const std::string& getCmakeFlags();

        Config();
    };
}