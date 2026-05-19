//
// Created by Nazım Can on 25.10.2025.
//

#pragma once

#include <string_view>
#include "math/math.h"
#include "wave.hpp"

namespace Waves
{
    class Semicircle final : public Wave
    {
    public:
        static constexpr std::string_view name = "Semicircle Wave";
        static HarmonicTerm formula(float i);
        static constexpr float dc() { return math::pi_v<float> / 4.f; }

        Semicircle();
        [[nodiscard]] HarmonicTerm get_formula(float i) const override;
        [[nodiscard]] float get_dc() const override;
    };
} // namespace Waves
