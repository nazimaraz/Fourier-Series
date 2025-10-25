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

std::pair<float, float> Sawtooth::get_formula(float i) const
{
    const auto n = i + 1;
    const auto coeff = 2.f / (n * math::pi_v<float>);
    const auto radius = 0.5f - settings_->get_radius() * (coeff / (2.f / math::pi_v<float>));
    // radius = constant_radius * (1 / (n * math::pi_v<float>));
    return { n, radius };
}
