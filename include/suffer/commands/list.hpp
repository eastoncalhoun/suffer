#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <suffer/core/package.hpp>
#include <suffer/core/registryHandler.hpp>
#include <suffer/utils/io.hpp>


namespace suffer::commands {
    void list(std::vector<std::string>& arguments);
}