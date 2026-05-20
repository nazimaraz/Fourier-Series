//
// Created by Nazım Can on 25.10.2025.
//

#include "triangle.hpp"
#include "math/math.h"

using namespace Waves;

HarmonicTerm Triangle::formula(const float i)
{
    const auto n = i * 2.f + 1.f;
    const auto coefficient = (8.f / math::square(math::pi_v<float>)) * (math::pow(-1.f, (n - 1.f) / 2.f) / math::square(n));
    return {n, coefficient, 0.f};
}
