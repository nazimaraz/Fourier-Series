//
// Created by Nazım Can on 25.10.2025.
//

#pragma once

#include <string_view>
#include "wave.hpp"

namespace Waves
{
    class Sawtooth final : public Wave
    {
    public:
        static constexpr std::string_view name = "Sawtooth Wave";
        static HarmonicTerm formula(float i);
        static constexpr float dc() { return 0.f; }

        Sawtooth();
        [[nodiscard]] HarmonicTerm get_formula(float i) const override;
        [[nodiscard]] float get_dc() const override;
    };
} // namespace Waves
