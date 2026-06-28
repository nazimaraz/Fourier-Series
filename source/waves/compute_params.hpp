//
// Created by nazim on 5/20/26.
//

#pragma once

#include <span>

namespace Waves
{
    struct ComputeParams
    {
        float radius{};
        unsigned int harmonic_count{};
        float phase{};
        std::span<const bool> enabled_mask;
    };
} // namespace Waves
