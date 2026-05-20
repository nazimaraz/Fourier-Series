//
// Created by Nazım Can on 25.10.2025.
//

#pragma once

#include <string_view>
#include "harmonic_term.hpp"
#include "math/math.h"

namespace Waves
{
    struct Semicircle
    {
        static constexpr std::string_view name = "Semicircle Wave";
        static HarmonicTerm formula(float i);
        static constexpr float dc() { return math::pi_v<float> / 4.f; }
    };
} // namespace Waves
