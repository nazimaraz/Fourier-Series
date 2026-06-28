//
// Created by nazim on 5/22/26.
//

#pragma once

#include <string_view>
#include "waves/harmonic_term.hpp"

namespace Waves
{
    struct Cosine
    {
        static constexpr auto name = "Cosine";
        static constexpr auto latex = std::string_view{R"(f(t)=\cos(t))"};

        [[nodiscard]] static auto formula(float i) -> HarmonicTerm;

        [[nodiscard]] static constexpr auto dc() -> float
        {
            return 0.f;
        }
    };
} // namespace Waves
