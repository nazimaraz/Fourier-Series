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
        static constexpr auto name = "Triangle Wave";
        static constexpr auto latex =
            std::string_view{R"(f(t)=\dfrac{8}{\pi^{2}}\sum_{n=1,3,5,\ldots}^{\infty}\dfrac{(-1)^{(n-1)/2}}{n^{2}}\sin(nt))"};

        [[nodiscard]] static auto formula(float i) -> HarmonicTerm;

        [[nodiscard]] static constexpr auto dc() -> float
        {
            return 0.f;
        }
    };
} // namespace Waves
