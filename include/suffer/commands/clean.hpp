#pragma once

#include <iostream>
#include <filesystem>

#include <suffer/utils/io.hpp>
#include <suffer/core/registryHandler.hpp>

namespace suffer::commands {
    void clean(std::vector<std::string>& arguments);
}