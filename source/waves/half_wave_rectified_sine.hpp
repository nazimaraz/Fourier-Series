//
// Created by nazim on 5/20/26.
//

#pragma once

#include "wave.hpp"

namespace Waves
{
    class HalfWaveRectifiedSine final : public Wave
    {
    public:
        HalfWaveRectifiedSine();
        [[nodiscard]] HarmonicTerm get_formula(float i) const override;
        [[nodiscard]] float get_dc() const override;
    };
} // namespace Waves
