//
// Created by nazim on 5/22/26.
//

#include "parabolic.hpp"
#include "math/math.hpp"

using namespace Waves;

auto Parabolic::formula(const float i) -> HarmonicTerm
{
    const auto n = 2.f * i + 1.f;
    const auto sign = math::pow(-1.f, i);
    const auto coefficient = (32.f / math::pow(math::pi_v<float>, 3.f)) * sign / math::pow(n, 3.f);
    constexpr auto phase = math::pi_v<float> / 2.f; // cosine basis -> phase-shifted sine
    return {.n = n, .coefficient = coefficient, .phase = phase};
}
