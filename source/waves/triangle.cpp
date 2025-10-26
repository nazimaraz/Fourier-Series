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
    const auto formula = (8 / math::pow(math::pi_v<float>, 2.f)) * (math::pow(-1.f, (n - 1) / 2) / math::pow(n, 2.f));
    return { n, formula };
}
