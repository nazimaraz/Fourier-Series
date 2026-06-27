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
        [[nodiscard]] static HarmonicTerm formula(float i);

        [[nodiscard]] static constexpr float dc()
        {
            return 0.f;
        }
    };
} // namespace Waves
