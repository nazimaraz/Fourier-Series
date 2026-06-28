//
// Created by Nazım Can on 25.10.2025.
//

#pragma once

#include <string_view>
#include "waves/harmonic_term.hpp"

namespace Waves
{
    struct Sawtooth
    {
        static constexpr auto name = "Sawtooth Wave";
        static constexpr std::string_view latex = R"(f(t)=\dfrac{2}{\pi}\sum_{n=1}^{\infty}\dfrac{(-1)^{n+1}}{n}\sin(nt))";
        [[nodiscard]] static auto formula(float i) -> HarmonicTerm;

        [[nodiscard]] static constexpr auto dc() -> float
        {
            return 0.f;
        }
    };
} // namespace Waves
