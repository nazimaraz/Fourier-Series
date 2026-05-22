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
        static constexpr std::string_view name = "Cosine";
        [[nodiscard]] static HarmonicTerm formula(float i);
        [[nodiscard]] static constexpr float dc() { return 0.f; }
    };
} // namespace Waves
