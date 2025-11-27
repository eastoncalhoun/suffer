#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "../core/package.hpp"
#include "../core/registryHandler.hpp"
#include "../utils/io.hpp"

namespace suffer::commands {
    void list(std::vector<std::string>& arguments);
}