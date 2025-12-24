#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <suffer/utils/io.hpp>
#include <suffer/core/package.hpp>
#include <suffer/core/registryHandler.hpp>

namespace suffer::commands {
    void uninstall(std::vector<std::string>& arguments);
}
