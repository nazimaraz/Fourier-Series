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
    static constexpr auto half_pi_v = pi_v<T> / 2;
    static constexpr auto half_pi = pi / 2;

    template <typename T>
    [[nodiscard]] constexpr T pow(const T number, const T power)
    {
        return std::pow(number, power);
    }

    template <typename T>
    [[nodiscard]] constexpr T square(const T num)
    {
        return pow<T>(num, 2);
    }

    template <typename T>
    [[nodiscard]] constexpr T sin(const T num)
    {
        return std::sin(num);
    }

    template <typename T>
    [[nodiscard]] constexpr T cos(const T num)
    {
        return std::cos(num);
    }

    template <typename T>
    [[nodiscard]] constexpr T abs(const T num)
    {
        return std::abs(num);
    }
} // namespace math
