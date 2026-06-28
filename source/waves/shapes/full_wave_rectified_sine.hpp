//
// Created by Nazım Can on 25.10.2025.
//

#pragma once

#include <string_view>
#include "waves/harmonic_term.hpp"
#include "math/math.hpp"

namespace Waves
{
    struct FullWaveRectifiedSine
    {
        static constexpr std::string_view name = "Full-Wave Rectified Sine";
        static constexpr std::string_view latex =
            R"(f(t)=\dfrac{2}{\pi}-\dfrac{4}{\pi}\sum_{n=1}^{\infty}\dfrac{1}{4n^{2}-1}\cos(2nt))";
        [[nodiscard]] static HarmonicTerm formula(float i);

        [[nodiscard]] static constexpr float dc()
        {
            return 2.f / math::pi_v<float>;
        }
    };
} // namespace Waves
