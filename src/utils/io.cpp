#include "./io.hpp"

std::string suffer::utils::io::red(const std::string& string) {
    return "\x1b[31m" + string + "\x1b[0m";
}

std::string suffer::utils::io::yellow(const std::string& string) {
    return "\x1b[33m" + string + "\x1b[0m";
}

std::string suffer::utils::io::blue(const std::string& string) {
    return "\x1b[34m" + string + "\x1b[0m";
}

std::string suffer::utils::io::magenta(const std::string& string) {
    return "\x1b[35m" + string + "\x1b[0m";
}

std::string suffer::utils::io::cyan(const std::string& string) {
    return "\x1b[36m" + string + "\x1b[0m";
}

std::string suffer::utils::io::green(const std::string& string) {
    return "\x1b[32m" + string + "\x1b[0m";
}

const std::string suffer::utils::io::error() {
    return suffer::utils::io::red("[ ERROR ]");
}

const std::string suffer::utils::io::warning() {
    return suffer::utils::io::yellow("[ WARNING ]");
}

const std::string suffer::utils::io::okay() {
    return suffer::utils::io::blue("[ OKAY ]");
}

const std::string suffer::utils::io::info() {
    return suffer::utils::io::cyan("[ INFO ]");
}

const std::string suffer::utils::io::pInfo() {
    return suffer::utils::io::magenta("[ Package ]");
}

const std::string suffer::utils::io::dataString(const std::string& data) {
    return suffer::utils::io::blue("< " + data + " >");
}

const std::string suffer::utils::io::commandString(const std::string& cmd) {
    return suffer::utils::io::yellow("$ ") + suffer::utils::io::blue(cmd);
}

const std::string suffer::utils::io::yesNo() {
    return suffer::utils::io::green("\n[ y(es) / n(o) ]\n");
}

const std::string suffer::utils::io::noYes() {
    return suffer::utils::io::green("\n[ n(o) / y(es) ]\n");
}