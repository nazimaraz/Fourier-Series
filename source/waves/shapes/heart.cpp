//
// Created by nazim on 5/22/26.
//

#include <array>
#include <cstddef>
#include "heart.hpp"
#include "waves/harmonic_term.hpp"
#include "math/math.hpp"

using namespace Waves;

HarmonicTerm Heart::formula(const float i)
{
    constexpr auto terms = std::array{
        HarmonicTerm{.n = -1.f, .coefficient = 1.00f, .phase = +math::half_pi_v<float>},
        HarmonicTerm{.n = -3.f, .coefficient = 0.24f, .phase = -math::half_pi_v<float>},
        HarmonicTerm{.n = +2.f, .coefficient = 0.20f, .phase = -math::half_pi_v<float>},
        HarmonicTerm{.n = -2.f, .coefficient = 0.20f, .phase = -math::half_pi_v<float>},
        HarmonicTerm{.n = +3.f, .coefficient = 0.08f, .phase = +math::half_pi_v<float>},
        HarmonicTerm{.n = +1.f, .coefficient = 0.04f, .phase = +math::half_pi_v<float>},
        HarmonicTerm{.n = +4.f, .coefficient = 0.04f, .phase = -math::half_pi_v<float>},
        HarmonicTerm{.n = -4.f, .coefficient = 0.04f, .phase = -math::half_pi_v<float>},
    };
    const auto idx = static_cast<std::size_t>(i);
    if (idx >= terms.size())
        return {.n = 0.f, .coefficient = 0.f, .phase = 0.f};

    return terms.at(idx);
}
