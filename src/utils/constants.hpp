#pragma once

#include <string>
#include <unordered_map>

enum SUFFER_SWITCHED {
    HELP,
    INSTALL,
    IMPORT,
    LIST,
    UNINSTALL,
    CLEAN,
    INIT,
    REGEN
};

inline std::unordered_map<std::string, int> SWITCHED = {
    {"help", HELP},
    {"install", INSTALL},
    {"import", IMPORT},
    {"list", LIST},
    {"uninstall", UNINSTALL},
    {"clean", CLEAN},
    {"init", INIT},
    {"regen", REGEN}
};

//track known flags somehow
