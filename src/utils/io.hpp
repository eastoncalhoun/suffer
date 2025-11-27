#pragma once

#include <string>

namespace suffer::utils::io {
    std::string red(const std::string& string);
    std::string yellow(const std::string& string);
    std::string blue(const std::string& string);
    std::string magenta(const std::string& string);
    std::string cyan(const std::string& string);
    std::string green(const std::string& string);
    
    const std::string error();
    const std::string warning();
    const std::string okay();
    const std::string info();
    const std::string pInfo();
    const std::string cInfo();
    //default: no
    const std::string yesNo();
    //default: yes
    const std::string noYes();

    const std::string dataString(const std::string& string);
    const std::string commandString(const std::string& cmd);
}