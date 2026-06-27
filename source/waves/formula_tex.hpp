//
// Created by nazim on 6/27/26.
//

#pragma once

#include <cstddef>
#include <string>
#include <string_view>
#include "wave_variant.hpp"

namespace Waves
{
    // Returns the closed-form Fourier series of the wave at `wave_index` as a LaTeX string.
    // `wave_index` follows the same ordering as `WaveVariant` / `wave_names`.
    [[nodiscard]] std::string_view formula_tex(size_t wave_index);

    // Builds a LaTeX string for the *instantiated* Fourier series the renderer is actually
    // drawing: amplitudes scaled by `radius`, the time argument expressed via omega = 2*pi*f,
    // `harmonic_count` terms, and terms disabled by `enabled_mask` (may be null = all on)
    // dropped. Only the first few contributing terms are expanded, the rest collapsed to \cdots.
    [[nodiscard]] std::string dynamic_formula_tex(const WaveVariant& wave, unsigned int harmonic_count, float radius,
        float frequency, const bool* enabled_mask);
} // namespace Waves
