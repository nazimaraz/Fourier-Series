//
// Created by nazim on 5/21/26.
//

#pragma once

#include <concepts>
#include <string_view>
#include "harmonic_term.hpp"

namespace Waves
{
    template <typename W>
    concept WaveShape = requires(const W& w, float i) {
        { W::name } -> std::convertible_to<std::string_view>;
        { w.dc() } -> std::same_as<float>;
        { w.formula(i) } -> std::same_as<HarmonicTerm>;
    };
} // namespace Waves
