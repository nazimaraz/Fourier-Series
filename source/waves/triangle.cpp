//
// Created by Nazım Can on 25.10.2025.
//

#include "triangle.hpp"
#include "math/math.h"
#include "gui/settings.hpp"
#include "function.h"

using namespace Waves;

Triangle::Triangle()
    : Wave{"Triangle Wave", Type::Triangle}
{}

std::pair<float, float> Triangle::get_formula(const float i) const
{
    const auto n = i * 2 + 1;
    const auto coeff = 8.f / (math::square(n) * math::square(math::pi_v<float>));
    const auto radius = math::pow(-1.f, (n - 1) / 2) * settings_->get_radius() * (coeff / (8.f / math::square(math::pi_v<float>)));
    return { n, radius };
}
