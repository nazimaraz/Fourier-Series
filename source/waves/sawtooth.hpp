//
// Created by Nazım Can on 25.10.2025.
//

#pragma once

#include <string_view>
#include "harmonic_term.hpp"

namespace Waves
{
    struct Sawtooth
    {
        static constexpr std::string_view name = "Sawtooth Wave";
        static HarmonicTerm formula(float i);
        static constexpr float dc() { return 0.f; }
    };
} // namespace Waves
