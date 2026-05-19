//
// Created by Nazım Can on 25.10.2025.
//

#pragma once

#include <string_view>
#include "math/math.h"
#include "wave.hpp"

namespace Waves
{
    class FullWaveRectifiedSine final : public Wave
    {
    public:
        static constexpr std::string_view name = "Full-Wave Rectified Sine";
        static HarmonicTerm formula(float i);
        static constexpr float dc() { return 2.f / math::pi_v<float>; }

        FullWaveRectifiedSine();
        [[nodiscard]] HarmonicTerm get_formula(float i) const override;
        [[nodiscard]] float get_dc() const override;
    };
} // namespace Waves
