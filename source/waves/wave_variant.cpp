//
// Created by nazim on 5/20/26.
//

#include <cassert>
#include <ranges>
#include "wave_variant.hpp"
#include "math/math.hpp"
#include "wave_shape.hpp"

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
        [[nodiscard]] ComputeResult compute_for(const ComputeParams& params)
        {
            ComputeResult result;
            result.steps.reserve(params.harmonic_count);
            auto position = raylib::Vector2{0.f, params.radius * W::dc()};
            for (const auto i : std::views::iota(0u, params.harmonic_count))
            {
                const auto [n, coefficient, phase] = W::formula(static_cast<float>(i));
                const auto enabled = params.enabled_mask == nullptr || params.enabled_mask[i];
                const auto effective_coefficient = enabled ? coefficient : 0.f;
                const auto radius = params.radius * effective_coefficient;
                const auto previous_position = position;
                const auto angle = n * 2.f * math::pi_v<float> * params.phase + phase;
                position += {radius * math::cos(angle), radius * math::sin(angle)};
                result.steps.emplace_back(previous_position, position, math::abs(radius));
            }

            result.tip = position;
            return result;
        }
    } // namespace detail

    WaveVariant make_wave_at(const size_t index)
    {
        assert(index < wave_count && "wave index out of range");
        static constexpr auto factories = detail::make_factories(std::make_index_sequence<wave_count>{});
        return factories[index]();
    }

    ComputeResult compute(const WaveVariant& wave, const ComputeParams& params)
    {
        return std::visit(
            [&]<typename W>(const W&) {
                return detail::compute_for<W>(params);
            },
            wave
        );
    }
} // namespace Waves
