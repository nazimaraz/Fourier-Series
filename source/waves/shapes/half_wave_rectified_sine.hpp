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
        static constexpr std::string_view latex =
            R"(f(t)=\dfrac{1}{\pi}+\dfrac{1}{2}\sin(t)-\dfrac{2}{\pi}\sum_{n=2,4,6,\ldots}^{\infty}\dfrac{1}{n^{2}-1}\cos(nt))";
        [[nodiscard]] static auto formula(float i) -> HarmonicTerm;

        [[nodiscard]] static constexpr auto dc() -> float
        {
            return 1.f / math::pi_v<float>;
        }
    };
} // namespace Waves
