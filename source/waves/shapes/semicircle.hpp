//
// Created by Nazım Can on 25.10.2025.
//

#pragma once

#include <string_view>
#include "waves/harmonic_term.hpp"
#include "math/math.hpp"

namespace Waves
{
    struct Semicircle
    {
        static constexpr std::string_view name = "Semicircle Wave";
        [[nodiscard]] static HarmonicTerm formula(float i);

        [[nodiscard]] static constexpr float dc()
        {
            return math::pi_v<float> / 4.f;
        }
    };
} // namespace Waves
