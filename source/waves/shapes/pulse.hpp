//
// Created by nazim on 5/22/26.
//

#pragma once

#include <array>
#include <cstddef>

namespace Waves
{
    template <int DutyPercent>
    struct Pulse
    {
        static_assert(DutyPercent > 0 && DutyPercent < 100, "duty cycle must lie in the open interval (0%, 100%)");

        static constexpr auto name_storage = [] {
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
        static constexpr std::string_view name{name_storage.data()};

        [[nodiscard]] static HarmonicTerm formula(const float i)
        {
            constexpr auto duty = static_cast<float>(DutyPercent) / 100.f;
            const auto n = i + 1.f;
            const auto coefficient = (2.f / (math::pi_v<float> * n)) * math::sin(math::pi_v<float> * n * duty);
            constexpr auto phase = math::pi_v<float> / 2.f;
            return {.n = n, .coefficient = coefficient, .phase = phase};
        }

        [[nodiscard]] static constexpr float dc()
        {
            return static_cast<float>(DutyPercent) / 100.f;
        }
    };

    using Pulse25 = Pulse<25>;
    using Pulse75 = Pulse<75>;
} // namespace Waves
