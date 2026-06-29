//
// Created by nazim on 5/20/26.
//

#pragma once

#include <cstddef>
#include <variant>
#include "compute_params.hpp"
#include "compute_result.hpp"
#include "shapes/ataturk_signature.hpp"
#include "shapes/cosine.hpp"
#include "shapes/full_wave_rectified_sine.hpp"
#include "shapes/half_wave_rectified_sine.hpp"
#include "shapes/heart.hpp"
#include "shapes/parabolic.hpp"
#include "shapes/pulse.hpp"
#include "shapes/sawtooth.hpp"
#include "shapes/semicircle.hpp"
#include "shapes/square.hpp"
#include "shapes/triangle.hpp"
#include "shapes/user_path.hpp"

namespace Waves
{
    using WaveVariant = std::variant<Sawtooth, Square, Triangle, Semicircle, HalfWaveRectifiedSine, FullWaveRectifiedSine,
        Parabolic, Cosine, Pulse25, Pulse75, Heart, AtaturkSignature, UserPath>;

    namespace detail
    {
        template <std::size_t... Is>
        [[nodiscard]] constexpr auto make_name_table(std::index_sequence<Is...> index_sequence) -> auto;
        template <std::size_t... Is>
        [[nodiscard]] constexpr auto make_cstr_name_table(std::index_sequence<Is...> index_sequence) -> auto;
        template <typename T>
        [[nodiscard]] constexpr auto index_of_impl() -> std::size_t;
    } // namespace detail

    [[nodiscard]] auto make_wave_at(std::size_t index) -> WaveVariant;
    [[nodiscard]] auto compute(const WaveVariant& wave, const ComputeParams& params) -> ComputeResult;

    template <typename T>
    inline constexpr auto index_of = detail::index_of_impl<T>();
    inline constexpr auto wave_count = std::variant_size_v<WaveVariant>;
    extern const std::array<std::string_view, wave_count> wave_names;
    extern const std::array<const char*, wave_count> wave_names_cstr;
} // namespace Waves

#include "wave_variant.tpp"
