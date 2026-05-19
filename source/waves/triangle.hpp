//
// Created by Nazım Can on 25.10.2025.
//

#pragma once

#include "wave.hpp"

namespace Waves
{
    class Triangle final : public Wave
    {
    public:
        Triangle();
        [[nodiscard]] HarmonicTerm get_formula(float i) const override;
    };
} // namespace Waves
