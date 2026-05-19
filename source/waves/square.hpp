//
// Created by Nazım Can on 25.10.2025.
//

#pragma once

#include "wave.hpp"

namespace Waves
{
    class Square final : public Wave
    {
    public:
        Square();
        [[nodiscard]] HarmonicTerm get_formula(float i) const override;
    };
} // namespace Waves
