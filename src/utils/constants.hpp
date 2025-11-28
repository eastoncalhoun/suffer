#pragma once

#include <string>
#include <map>

enum {
    HELP,
    INSTALL,
    IMPORT,
    LIST,
    UNINSTALL
};

inline std::map<std::string, int> SWITCHED = {
    {"help", HELP},
    {"install", INSTALL},
    {"import", IMPORT},
    {"list", LIST},
    {"uninstall", UNINSTALL}
};

//track known flags somehow
