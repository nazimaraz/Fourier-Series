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
    [[nodiscard]] constexpr auto pow(const T number, const T power) -> T
    {
        return std::pow(number, power);
    }

    template <typename T>
    [[nodiscard]] constexpr auto square(const T num) -> T
    {
        return pow<T>(num, 2);
    }

    template <typename T>
    [[nodiscard]] constexpr auto sin(const T num) -> T
    {
        return std::sin(num);
    }

    template <typename T>
    [[nodiscard]] constexpr auto cos(const T num) -> T
    {
        return std::cos(num);
    }

    template <typename T>
    [[nodiscard]] constexpr auto abs(const T num) -> T
    {
        return std::abs(num);
    }
} // namespace math
