//
// Created by nazim on 5/26/26.
//

#pragma once

#include <cstddef>
#include <string_view>
#include <vector>
#include "waves/harmonic_term.hpp"

namespace Waves
{
    struct UserPath
    {
        static constexpr std::string_view name = "User Drawing";

        std::vector<HarmonicTerm> harmonics;
        float dc_value{};

        [[nodiscard]] HarmonicTerm formula(float i) const;
        [[nodiscard]] float dc() const;
    };
} // namespace Waves
