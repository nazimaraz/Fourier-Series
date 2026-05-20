//
// Created by Nazım Can on 25.10.2025.
//

#include "sawtooth.hpp"
#include "math/math.h"

using namespace Waves;

HarmonicTerm Sawtooth::formula(const float i)
{
    const auto n = i + 1.f;
    const auto coefficient = 2.f / (math::pi_v<float> * n);
    return {n, coefficient, 0.f};
}
