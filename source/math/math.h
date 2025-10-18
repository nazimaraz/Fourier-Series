//
// Created by nazim on 18/10/2025.
//

#pragma once

#include <cmath>
#include <numbers>

namespace math
{
    template <typename T>
    static constexpr auto pi_v = std::numbers::pi_v<T>;
    static constexpr auto pi = pi_v<double>;

    template <typename T>
    constexpr T sin(T const num)
    {
        return std::sin(num);
    }

    template <typename T>
    constexpr T cos(T const num)
    {
        return std::cos(num);
    }
}
