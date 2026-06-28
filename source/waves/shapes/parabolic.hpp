//
// Created by nazim on 5/22/26.
//

#pragma once

#include <string_view>
#include "waves/harmonic_term.hpp"

namespace Waves
{
    struct Parabolic
    {
        static constexpr std::string_view name = "Parabolic Wave";
        static constexpr std::string_view latex =
            R"(f(t)=\dfrac{32}{\pi^{3}}\sum_{n=1,3,5,\ldots}^{\infty}\dfrac{(-1)^{(n-1)/2}}{n^{3}}\cos(nt))";
        [[nodiscard]] static HarmonicTerm formula(float i);

        [[nodiscard]] static constexpr float dc()
        {
            return 0.f;
        }
    };
} // namespace Waves
