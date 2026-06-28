//
// Created by nazim on 5/26/26.
//

#pragma once

#include <cstddef>
#include <raylib.h>
#include <vector>
#include "harmonic_term.hpp"

namespace Waves
{
    struct DftResult
    {
        std::vector<HarmonicTerm> harmonics;
        float dc_value;
    };

    [[nodiscard]] auto compute_dft(const std::vector<Vector2>& points, std::size_t max_harmonics) -> DftResult;
} // namespace Waves
