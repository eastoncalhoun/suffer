#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "../utils/io.hpp"
#include "../core/registryHandler.hpp"
#include "../core/package.hpp"

namespace suffer::commands {
    void uninstall(std::vector<std::string>& arguments);
}