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

std::pair<float, float> Sawtooth::get_formula(const float i) const
{
    const auto n = i + 1;
    const auto formula = /*0.5f - */(1 / math::pi_v<float>) * (1 / n);
    return { n, formula };
}
