#pragma once

#include <string>
#include <vector>

#include "./install.hpp"

namespace suffer::commands {
    class Router {
    private:
        std::vector<std::string> arguments;

    public:
        void route();

        Router(int argc, char** argv);
    };
}