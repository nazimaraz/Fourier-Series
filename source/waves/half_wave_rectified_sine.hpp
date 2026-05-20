//
// Created by nazim on 5/20/26.
//

#pragma once

#include <string_view>
#include "harmonic_term.hpp"
#include "math/math.h"

namespace Waves
{
    struct HalfWaveRectifiedSine
    {
        static constexpr std::string_view name = "Half-Wave Rectified Sine";
        static HarmonicTerm formula(float i);
        static constexpr float dc() { return 1.f / math::pi_v<float>; }
    };
} // namespace Waves
