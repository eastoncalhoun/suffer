#pragma once

#include <iostream>
#include <filesystem>

#include "../utils/io.hpp"
#include "../core/registryHandler.hpp"

namespace suffer::commands {
    void clean(std::vector<std::string>& arguments);
}