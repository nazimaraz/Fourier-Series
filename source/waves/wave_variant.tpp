#pragma once

#include <array>
#include <cstddef>

namespace Waves
{
    namespace detail
    {
        template <std::size_t... Is>
        [[nodiscard]] constexpr auto make_name_table(std::index_sequence<Is...> /*index_sequence*/) -> auto
        {
            return std::array<std::string_view, wave_count>{std::variant_alternative_t<Is, WaveVariant>::name...};
        }

        template <std::size_t... Is>
        [[nodiscard]] constexpr auto make_cstr_name_table(std::index_sequence<Is...> /*index_sequence*/) -> auto
        {
            return std::array<const char*, wave_count>{std::variant_alternative_t<Is, WaveVariant>::name...};
        }

        template <typename T>
        [[nodiscard]] constexpr auto index_of_impl() -> std::size_t
        {
            auto result = wave_count;
            [&]<std::size_t... Is>(std::index_sequence<Is...>) -> void {
                ((std::is_same_v<T, std::variant_alternative_t<Is, WaveVariant>> ? (result = Is, 0) : 0), ...);
            }(std::make_index_sequence<wave_count>{});
            return result;
        }
    } // namespace detail

    inline constexpr auto wave_names = detail::make_name_table(std::make_index_sequence<wave_count>{});
    inline constexpr auto wave_names_cstr = detail::make_cstr_name_table(std::make_index_sequence<wave_count>{});
} // namespace Waves
