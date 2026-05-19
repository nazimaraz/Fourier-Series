//
// Created by nazim on 5/20/26.
//

#pragma once

#include <string_view>
#include "math/math.h"
#include "wave.hpp"

namespace Waves
{
    class HalfWaveRectifiedSine final : public Wave
    {
    public:
        static constexpr std::string_view name = "Half-Wave Rectified Sine";
        static HarmonicTerm formula(float i);
        static constexpr float dc() { return 1.f / math::pi_v<float>; }

        HalfWaveRectifiedSine();
        [[nodiscard]] HarmonicTerm get_formula(float i) const override;
        [[nodiscard]] float get_dc() const override;
    };
} // namespace Waves
