//
// Created by nazim on 5/20/26.
//

#include <ranges>
#include "wave_variant.hpp"
#include "math/math.hpp"
#include "wave_shape.hpp"
#include <raymath.h>

namespace Waves
{
    namespace detail
    {
        template <size_t... Is>
        [[nodiscard]] constexpr auto make_factories(std::index_sequence<Is...>)
        {
            return std::array<WaveVariant (*)(), sizeof...(Is)>{+[]() -> WaveVariant {
                return WaveVariant{std::in_place_index<Is>};
            }...};
        }

        template <WaveShape W>
        [[nodiscard]] ComputeResult compute_for(const W& wave, const ComputeParams& params)
        {
            ComputeResult result;
            result.steps.reserve(params.harmonic_count);
            auto position = Vector2{0.f, params.radius * wave.dc()};
            for (const auto i : std::views::iota(0u, params.harmonic_count))
            {
                const auto [n, coefficient, phase] = wave.formula(static_cast<float>(i));
                const auto enabled = params.enabled_mask == nullptr || params.enabled_mask[i];
                const auto effective_coefficient = enabled ? coefficient : 0.f;
                const auto radius = params.radius * effective_coefficient;
                const auto previous_position = position;
                const auto angle = n * 2.f * math::pi_v<float> * params.phase + phase;
                position = Vector2Add(position, {radius * math::cos(angle), radius * math::sin(angle)});
                result.steps.emplace_back(previous_position, position, math::abs(radius));
            }

            result.tip = position;
            return result;
        }
    } // namespace detail

    WaveVariant make_wave_at(const size_t index)
    {
        static constexpr auto factories = detail::make_factories(std::make_index_sequence<wave_count>{});
        return factories.at(index)();
    }

    ComputeResult compute(const WaveVariant& wave, const ComputeParams& params)
    {
        return std::visit(
            [&](const auto& w) {
                return detail::compute_for(w, params);
            },
            wave
        );
    }
} // namespace Waves
