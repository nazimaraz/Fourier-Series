//
// Created by nazim on 5/20/26.
//

#pragma once

#include <variant>
#include "compute_params.hpp"
#include "compute_result.hpp"
#include "full_wave_rectified_sine.hpp"
#include "half_wave_rectified_sine.hpp"
#include "sawtooth.hpp"
#include "semicircle.hpp"
#include "square.hpp"
#include "triangle.hpp"

namespace Waves
{
    using WaveVariant = std::variant<Sawtooth, Square, Triangle, Semicircle, HalfWaveRectifiedSine, FullWaveRectifiedSine>;

    namespace detail
    {
        template <size_t... Is>
        constexpr auto make_name_table(std::index_sequence<Is...>);
        template <size_t... Is>
        constexpr auto make_cstr_name_table(std::index_sequence<Is...>);
        template <typename T>
        constexpr size_t index_of_impl();
    } // namespace detail

    [[nodiscard]] WaveVariant make_wave_at(size_t index);
    [[nodiscard]] ComputeResult compute(const WaveVariant& wave, const ComputeParams& params);

    template <typename T>
    inline constexpr auto index_of = detail::index_of_impl<T>();
    inline constexpr auto wave_count = std::variant_size_v<WaveVariant>;
    extern const std::array<std::string_view, wave_count> wave_names;
    extern const std::array<const char*, wave_count> wave_names_cstr;
} // namespace Waves

#include "wave_variant.tpp"
