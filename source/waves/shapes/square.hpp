//
// Created by Nazım Can on 25.10.2025.
//

#pragma once

#include <string_view>
#include "waves/harmonic_term.hpp"

namespace Waves
{
    struct Square
    {
        static constexpr std::string_view name = "Square Wave";
        [[nodiscard]] static HarmonicTerm formula(float i);
        [[nodiscard]] static constexpr float dc()
        {
            return 0.f;
        }
    };
} // namespace Waves
