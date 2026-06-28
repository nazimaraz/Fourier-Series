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
        static constexpr std::string_view latex =
            R"(f(t)=\dfrac{\pi}{4}+\sum_{n=1}^{\infty}\dfrac{(-1)^{n}}{n}J_{1}(n\pi)\cos(nt))";
        [[nodiscard]] static HarmonicTerm formula(float i);

        [[nodiscard]] static constexpr float dc()
        {
            return math::pi_v<float> / 4.f;
        }
    };
} // namespace Waves
