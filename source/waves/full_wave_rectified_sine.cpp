//
// Created by Nazım Can on 25.10.2025.
//

#include "full_wave_rectified_sine.hpp"
#include "math/math.h"

using namespace Waves;

HarmonicTerm FullWaveRectifiedSine::formula(const float i)
{
    const auto k = i + 1.f;
    const auto n = 2.f * k;
    const auto coefficient = -4.f / (math::pi_v<float> * (4.f * k * k - 1.f));
    constexpr auto phase = math::pi_v<float> / 2.f;
    return {n, coefficient, phase};
}
