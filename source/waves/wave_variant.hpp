//
// Created by nazim on 5/20/26.
//

#pragma once

#include <variant>

#include "compute_params.hpp"
#include "compute_result.hpp"
#include "full_wave_rectified_sine.hpp"
#include "half_wave_rectified_sine.hpp"
#include "sawtooth.hpp"
#include "semicircle.hpp"
#include "square.hpp"
#include "triangle.hpp"

namespace Waves
{

    using WaveVariant = std::variant<
        Sawtooth,
        Square,
        Triangle,
        Semicircle,
        HalfWaveRectifiedSine,
        FullWaveRectifiedSine>;

    [[nodiscard]] ComputeResult compute(const WaveVariant& wave, const ComputeParams& params);

} // namespace Waves
