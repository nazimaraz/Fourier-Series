//
// Created by nazim on 5/20/26.
//

#pragma once

#include <string_view>
#include "waves/harmonic_term.hpp"
#include "math/math.hpp"

namespace Waves
{
    struct HalfWaveRectifiedSine
    {
        static constexpr std::string_view name = "Half-Wave Rectified Sine";
        [[nodiscard]] static HarmonicTerm formula(float i);

        [[nodiscard]] static constexpr float dc()
        {
            return 1.f / math::pi_v<float>;
        }
    };
} // namespace Waves
