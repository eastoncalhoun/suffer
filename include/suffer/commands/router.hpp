#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <suffer/utils/constants.hpp>
#include <suffer/commands/install.hpp>
#include <suffer/commands/import.hpp>
#include <suffer/commands/list.hpp>
#include <suffer/commands/help.hpp>
#include <suffer/commands/uninstall.hpp>
#include <suffer/commands/clean.hpp>
#include <suffer/commands/init.hpp>
#include <suffer/commands/regen.hpp>

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