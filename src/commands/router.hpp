#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "../utils/constants.hpp"
#include "./install.hpp"
#include "./import.hpp"
#include "./list.hpp"
#include "./help.hpp"
#include "./uninstall.hpp"
#include "./clean.hpp"
#include "./init.hpp"
#include "./regen.hpp"

namespace suffer::commands {
    class Router {
    private:
        std::string command;
        std::vector<std::string> arguments;

    public:
        void route();

        Router(int argc, char** argv);
    };
}