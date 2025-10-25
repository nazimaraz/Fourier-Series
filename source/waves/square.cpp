//
// Created by Nazım Can on 25.10.2025.
//

#include "square.hpp"
#include "math/math.h"

using namespace Waves;

Square::Square()
    : Wave{"Square Wave", Type::Square}
{}

std::pair<float, float> Square::get_formula(float i) const
{
    const auto n = i * 2 + 1;
    const auto coeff = 4.f / (n * math::pi_v<float>);
    const auto radius = settings_->get_radius() * (coeff / (4.f / math::pi_v<float>));
    // radius = constant_radius * (4 / (n * math::pi_v<float>));
    return { n, radius };
}
