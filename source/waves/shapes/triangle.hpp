//
// Created by Nazım Can on 25.10.2025.
//

#pragma once

#include <string_view>
#include "waves/harmonic_term.hpp"

namespace Waves
{
    struct Triangle
    {
        static constexpr std::string_view name = "Triangle Wave";
        static constexpr std::string_view latex =
            R"(f(t)=\dfrac{8}{\pi^{2}}\sum_{n=1,3,5,\ldots}^{\infty}\dfrac{(-1)^{(n-1)/2}}{n^{2}}\sin(nt))";
        [[nodiscard]] static HarmonicTerm formula(float i);

        [[nodiscard]] static constexpr float dc()
        {
            return 0.f;
        }
    };
} // namespace Waves
