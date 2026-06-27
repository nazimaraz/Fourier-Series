//
// Created by nazim on 6/27/26.
//

#pragma once

#include <cstddef>
#include <string_view>

namespace Waves
{
    // Returns the closed-form Fourier series of the wave at `wave_index` as a LaTeX string.
    // `wave_index` follows the same ordering as `WaveVariant` / `wave_names`.
    [[nodiscard]] std::string_view formula_tex(size_t wave_index);
} // namespace Waves
