//
// Created by Nazım Can on 25.10.2025.
//

#pragma once

#include "wave.hpp"

namespace Waves
{
    class Semicircle final : public Wave
    {
    public:
        Semicircle();
        [[nodiscard]] std::pair<float, float> get_formula(float i) const override;
    };
} // namespace Waves
