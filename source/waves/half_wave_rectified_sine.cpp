//
// Created by nazim on 5/20/26.
//

#include "half_wave_rectified_sine.hpp"
#include "math/math.h"
#include "gui/settings.hpp"
#include "function.h"

using namespace Waves;

HalfWaveRectifiedSine::HalfWaveRectifiedSine()
    : Wave{"Half-Wave Rectified Sine", Type::HalfWaveRectifiedSine}
{}

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

HarmonicTerm HalfWaveRectifiedSine::get_formula(const float i) const
{
    return formula(i);
}

float HalfWaveRectifiedSine::get_dc() const
{
    return dc();
}
