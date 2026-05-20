//
// Created by nazim on 5/20/26.
//

#include "half_wave_rectified_sine.hpp"
#include "math/math.hpp"

using namespace Waves;

HarmonicTerm HalfWaveRectifiedSine::formula(const float i)
{
    if (i == 0.f)
        return {1.f, 0.5f, 0.f};

    const auto k = i;
    const auto n = 2.f * k;
    const auto coefficient = -2.f / (math::pi_v<float> * (4.f * k * k - 1.f));
    constexpr auto phase = math::pi_v<float> / 2.f;
    return {n, coefficient, phase};
}
