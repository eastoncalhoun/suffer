#pragma once

#include <vector>
#include <string>

#include <suffer/utils/io.hpp>
#include <suffer/core/package.hpp>
#include <suffer/core/registryHandler.hpp>
#include <suffer/core/builder.hpp>

namespace suffer::commands {
    void import(std::vector<std::string>& args);
}