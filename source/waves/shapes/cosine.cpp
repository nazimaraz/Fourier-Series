//
// Created by nazim on 5/22/26.
//

#include "cosine.hpp"
#include "math/math.hpp"

using namespace Waves;

auto Cosine::formula(const float i) -> HarmonicTerm
{
    if (i == 0.f)
        return {.n = 1.f, .coefficient = 1.f, .phase = math::pi_v<float> / 2.f};

    return {.n = 0.f, .coefficient = 0.f, .phase = 0.f};
}
