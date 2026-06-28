//
// Created by Nazım Can on 25.10.2025.
//

#include <boost/math/special_functions/bessel.hpp>
#include "semicircle.hpp"
#include "math/math.hpp"

using namespace Waves;

auto Semicircle::formula(const float i) -> HarmonicTerm
{
    const auto n = i + 1.f;
    const auto coefficient = math::pow(-1.f, n) * boost::math::cyl_bessel_j(1, n * math::pi_v<float>) / n;
    constexpr auto phase = math::pi_v<float> / 2.f;
    return {.n = n, .coefficient = coefficient, .phase = phase};
}
