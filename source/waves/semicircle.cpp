//
// Created by Nazım Can on 25.10.2025.
//

#include <boost/math/special_functions/bessel.hpp>
#include "semicircle.hpp"
#include "math/math.h"
#include "gui/settings.hpp"
#include "function.h"

using namespace Waves;

Semicircle::Semicircle()
    : Wave{"Semicircle Wave", Type::Semicircle}
{}

HarmonicTerm Semicircle::get_formula(const float i) const
{
    const auto n = i + 1.f;
    const auto coefficient = math::pow(-1.f, n) * boost::math::cyl_bessel_j(1, n * math::pi_v<float>) / n;
    constexpr auto phase = math::pi_v<float> / 2.f;
    return {n, coefficient, phase};
}

float Semicircle::get_dc() const
{
    return math::pi_v<float> / 4.f;
}
