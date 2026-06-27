//
// Created by nazim on 5/20/26.
//

#pragma once

#include <vector>
#include "epicycle_step.hpp"

namespace Waves
{
    struct ComputeResult
    {
        std::vector<EpicycleStep> steps;
        Vector2 tip;
    };
} // namespace Waves
