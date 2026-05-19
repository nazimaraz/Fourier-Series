//
// Created by Nazım Can on 25.10.2025.
//

#include "sawtooth.hpp"
#include "math/math.h"
#include "gui/settings.hpp"
#include "function.h"

using namespace Waves;

Sawtooth::Sawtooth()
    : Wave{"Sawtooth Wave", Type::Sawtooth}
{}

HarmonicTerm Sawtooth::formula(const float i)
{
    const auto n = i + 1.f;
    const auto coefficient = 2.f / (math::pi_v<float> * n);
    return {n, coefficient, 0.f};
}

HarmonicTerm Sawtooth::get_formula(const float i) const
{
    return formula(i);
}

float Sawtooth::get_dc() const
{
    return dc();
}
