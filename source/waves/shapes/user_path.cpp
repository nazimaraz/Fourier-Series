//
// Created by nazim on 5/26/26.
//

#include <cstddef>
#include "user_path.hpp"

using namespace Waves;

auto UserPath::formula(const float i) const -> HarmonicTerm
{
    const auto idx = static_cast<std::size_t>(i);
    if (idx >= harmonics.size())
        return {.n = 0.f, .coefficient = 0.f, .phase = 0.f};

    return harmonics[idx];
}

auto UserPath::dc() const -> float
{
    return dc_value;
}
