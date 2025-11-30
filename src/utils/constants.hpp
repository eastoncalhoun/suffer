#pragma once

#include <string>
#include <unordered_map>

enum {
    HELP,
    INSTALL,
    IMPORT,
    LIST,
    UNINSTALL,
    CLEAN,
    INIT
};

inline std::unordered_map<std::string, int> SWITCHED = {
    {"help", HELP},
    {"install", INSTALL},
    {"import", IMPORT},
    {"list", LIST},
    {"uninstall", UNINSTALL},
    {"clean", CLEAN},
    {"init", INIT}
};

//track known flags somehow
