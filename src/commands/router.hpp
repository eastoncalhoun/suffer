#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "./constants.hpp"
#include "./install.hpp"
#include "./import.hpp"
#include "./list.hpp"
#include "./help.hpp"
#include "./uninstall.hpp"

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