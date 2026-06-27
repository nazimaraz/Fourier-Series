//
// Created by nazim on 6/27/26.
//

#pragma once

#include <string>
#include <string_view>

namespace Waves
{
    // Returns the closed-form Fourier series of the wave at `wave_index` as a LaTeX string.
    // `wave_index` follows the same ordering as `WaveVariant` / `wave_names`.
    [[nodiscard]] std::string_view formula_tex(size_t wave_index);

    // Builds the *dynamic* LaTeX string by taking the original closed-form series of the wave at
    // `wave_index` and substituting the live values into it: the infinite sum bound becomes the
    // harmonic count, the time variable becomes omega*t (omega defined afterwards), and the whole
    // series is scaled by `radius` when it differs from 1. The original formula structure is kept
    // intact — only the dynamic values are plugged in.
    [[nodiscard]] std::string dynamic_formula_tex(size_t wave_index, unsigned int harmonic_count, float radius, float frequency);
} // namespace Waves
