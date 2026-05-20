//
// Created by Nazım Can on 25.10.2025.
//

#include "square.hpp"
#include "math/math.h"

using namespace Waves;

HarmonicTerm Square::formula(const float i)
{
    const auto n = i * 2.f + 1.f;
    const auto coefficient = 4.f / (math::pi_v<float> * n);
    return {n, coefficient, 0.f};
}
