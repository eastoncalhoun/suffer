#pragma once

#include <vector>
#include <string>

#include "../utils/io.hpp"
#include "../core/package.hpp"
#include "../core/registryHandler.hpp"
#include "../core/builder.hpp"

namespace suffer::commands {
    void import(std::vector<std::string>& args);
}