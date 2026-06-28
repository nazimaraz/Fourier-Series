//
// Created by Nazım Can on 25.10.2025.
//

#include "sawtooth.hpp"
#include "math/math.hpp"

using namespace Waves;

auto Sawtooth::formula(const float i) -> HarmonicTerm
{
    const auto n = i + 1.f;
    const auto coefficient = 2.f / (math::pi_v<float> * n);
    return {.n = n, .coefficient = coefficient, .phase = 0.f};
}
