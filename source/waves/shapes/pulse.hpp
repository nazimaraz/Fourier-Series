//
// Created by nazim on 5/22/26.
//

#pragma once

#include <array>
#include <cstddef>
#include <string_view>

namespace Waves
{
    template <int DutyPercent>
    struct Pulse
    {
        static_assert(DutyPercent > 0 && DutyPercent < 100, "duty cycle must lie in the open interval (0%, 100%)");

        static constexpr auto name_storage = [] -> std::array<char, 16> {
            constexpr std::string_view prefix = "Pulse (";
            auto result = std::array<char, 16>{};
            auto i = std::size_t{};
            for (const auto c : prefix)
                result.at(i++) = c;

            if constexpr (DutyPercent >= 10)
            {
                result.at(i++) = static_cast<char>('0' + DutyPercent / 10);
                result.at(i++) = static_cast<char>('0' + DutyPercent % 10);
            }
            else
            {
                result.at(i++) = static_cast<char>('0' + DutyPercent);
            }

            result.at(i++) = '%';
            result.at(i++) = ')';
            return result;
        }();
        static constexpr auto name = std::string_view{name_storage.data()};
        static constexpr auto latex = [] -> std::string_view {
            if constexpr (DutyPercent == 25)
                return R"(f(t)=0.25+\dfrac{2}{\pi}\sum_{n=1}^{\infty}\dfrac{\sin(0.25\pi n)}{n}\cos(nt))";
            else if constexpr (DutyPercent == 75)
                return R"(f(t)=0.75+\dfrac{2}{\pi}\sum_{n=1}^{\infty}\dfrac{\sin(0.75\pi n)}{n}\cos(nt))";
            else
                return {};
        }();

        [[nodiscard]] static auto formula(const float i) -> HarmonicTerm
        {
            constexpr auto duty = static_cast<float>(DutyPercent) / 100.f;
            const auto n = i + 1.f;
            const auto coefficient = (2.f / (math::pi_v<float> * n)) * math::sin(math::pi_v<float> * n * duty);
            constexpr auto phase = math::pi_v<float> / 2.f;
            return {.n = n, .coefficient = coefficient, .phase = phase};
        }

        [[nodiscard]] static constexpr auto dc() -> float
        {
            return static_cast<float>(DutyPercent) / 100.f;
        }
    };

    using Pulse25 = Pulse<25>;
    using Pulse75 = Pulse<75>;
} // namespace Waves
