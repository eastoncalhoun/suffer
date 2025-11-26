#pragma once

#include <vector>
#include <string>

#include "../utils/io.hpp"
#include "../core/package.hpp"
#include "../core/registryHandler.hpp"

namespace suffer::commands {
    void install(std::vector<std::string>& args);
}