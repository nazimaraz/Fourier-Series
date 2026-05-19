//
// Created by Nazım Can on 25.10.2025.
//

#pragma once

#include "wave.hpp"

namespace Waves
{
    class Sawtooth final : public Wave
    {
    public:
        Sawtooth();
        [[nodiscard]] HarmonicTerm get_formula(float i) const override;
    };
} // namespace Waves
