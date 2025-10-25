//
// Created by Nazım Can on 25.10.2025.
//

#include <boost/math/special_functions/bessel.hpp>
#include "semicircle.hpp"
#include "math/math.h"

using namespace Waves;

Semicircle::Semicircle()
    : Wave{"Semicircle Wave", Type::Semicircle}
{}

std::pair<float, float> Semicircle::get_formula(float i) const
{
    const auto n = i * 2 + 1;
    const auto coeff = math::pi_v<float> * math::pow(-1.f, n) * boost::math::cyl_bessel_j(1, n * math::pi_v<float>) / (4 * n);
    const auto radius = settings_->get_radius() * (coeff / (math::pi_v<float> / 4));
    // radius = constant_radius * (4 / (n * math::pi_v<float>));
    return { n, radius };
}
