//
// Created by Nazım Can on 25.10.2025.
//

#include "full_wave_rectified_sine.hpp"
#include "math/math.h"
#include "gui/settings.hpp"
#include "function.h"

using namespace Waves;

FullWaveRectifiedSine::FullWaveRectifiedSine()
    : Wave{"Full-Wave Rectified Sine", Type::FullWaveRectifiedSine}
{}

HarmonicTerm FullWaveRectifiedSine::get_formula(const float i) const
{
    const auto k = i + 1.f;
    const auto n = 2.f * k;
    const auto coefficient = -4.f / (math::pi_v<float> * (4.f * k * k - 1.f));
    constexpr auto phase = math::pi_v<float> / 2.f;
    return {n, coefficient, phase};
}

float FullWaveRectifiedSine::get_dc() const
{
    return 2.f / math::pi_v<float>;
}
