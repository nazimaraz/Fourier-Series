//
// Created by nazim on 5/22/26.
//

#pragma once

#include <string_view>
#include "waves/harmonic_term.hpp"

namespace Waves
{
    struct Heart
    {
        static constexpr auto name = "Heart";
        static constexpr auto latex = std::string_view{R"(\bigl{(}x(t),y(t)\bigr{)}=\sum_{n}(a_{n}\cos nt+b_{n}\sin nt))"};

        [[nodiscard]] static auto formula(float i) -> HarmonicTerm;

        [[nodiscard]] static constexpr auto dc() -> float
        {
            return 0.f;
        }
    };
} // namespace Waves
