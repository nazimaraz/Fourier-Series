//
// Created by nazim on 6/29/26.
//

#pragma once

#include <string_view>
#include "waves/harmonic_term.hpp"

namespace Waves
{
    struct AtaturkSignature
    {
        static constexpr auto name = "Atatürk Signature";
        static constexpr auto latex = std::string_view{R"(\text{Ataturk signature})"};

        [[nodiscard]] static auto formula(float i) -> HarmonicTerm;

        [[nodiscard]] static constexpr auto dc() -> float
        {
            return 0.f;
        }
    };
} // namespace Waves
