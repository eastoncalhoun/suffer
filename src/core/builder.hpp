#pragma once

#include <filesystem>
#include <string>
#include <map>

#include "./package.hpp"

namespace suffer::core {
    enum {
        IH_H_HEADER_STYLE,
        SI_H_HEADER_STYLE,
        RT_H_STYLE,
        UNKNOWN
    };

    class Builder {
    private:
        const suffer::core::Package& package;
        int determineHeaderOnlyPackaging();
    public:
        Builder(const suffer::core::Package& package);
    };
}