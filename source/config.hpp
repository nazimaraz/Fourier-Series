//
// Created by nazim on 5/26/26.
//

#pragma once

#include <cstddef>
#include <cstdint>
#include <raylib.h>

namespace Config
{
    namespace Window
    {
        inline constexpr auto screen_width = 1600;
        inline constexpr auto screen_height = 900;
        inline constexpr auto target_fps = 120;
    } // namespace Window

    namespace Defaults
    {
        inline constexpr auto background_color = Color{.r = 11, .g = 13, .b = 16, .a = 255};
        inline constexpr auto wave_capacity = 1000;
        inline constexpr auto path_capacity = 4000;
        inline constexpr auto max_harmonic_count = std::size_t{100};
        inline constexpr auto harmonic_count = 10u;
        inline constexpr auto frequency = 0.1f;
        inline constexpr auto radius = 100.f;
        inline constexpr auto x_scale = 1.f;
        inline constexpr auto y_scale = 1.f;
    } // namespace Defaults

    namespace Chart
    {
        inline constexpr auto origin_x = 800.f;
        inline constexpr auto origin_y = 450.f;
        inline constexpr auto chart_left = 200.f;
        inline constexpr auto grid_step_px = 80.f;
        inline constexpr auto tick_len = 4.f;
        inline constexpr auto axis_color = Color{.r = 105, .g = 116, .b = 130, .a = 210};
        inline constexpr auto grid_color = Color{.r = 35, .g = 42, .b = 49, .a = 145};
        inline constexpr auto label_color = Color{.r = 174, .g = 185, .b = 197, .a = 255};
        inline constexpr auto label_font = 11;
    } // namespace Chart

    namespace Wave
    {
        inline constexpr auto epicycle_origin_x = 600.f;
        inline constexpr auto epicycle_origin_y = 450.f;
        inline constexpr auto substeps_per_cycle = 1000.f;
        inline constexpr auto substeps_max = 64;
        inline constexpr auto wave_color = Color{.r = 255, .g = 90, .b = 95, .a = 255};
        inline constexpr auto epicycle_color = Color{.r = 232, .g = 238, .b = 245, .a = 245};
        inline constexpr auto circle_color = Color{.r = 183, .g = 195, .b = 208, .a = 95};
        inline constexpr auto path_color = Color{.r = 255, .g = 154, .b = 138, .a = 205};
    } // namespace Wave

    namespace Formula
    {
        [[nodiscard]] constexpr auto to_argb(const Color color) -> std::uint32_t
        {
            return (static_cast<std::uint32_t>(color.a) << 24u) | (static_cast<std::uint32_t>(color.r) << 16u) |
                   (static_cast<std::uint32_t>(color.g) << 8u) | static_cast<std::uint32_t>(color.b);
        }

        inline constexpr auto text_size = 26.f;
        inline constexpr auto dynamic_text_size = 22.f;
        inline constexpr auto margin_top = 14.f;
        inline constexpr auto panel_gap = 16.f;
        inline constexpr auto panel_padding = 12.f;
        inline constexpr auto panel_radius = 0.06f;
        inline constexpr auto vertical_gap = 6.f;
        inline constexpr auto panel_background = Color{.r = 20, .g = 24, .b = 29, .a = 220};
        inline constexpr auto panel_border = Color{.r = 55, .g = 64, .b = 74, .a = 190};
        inline constexpr auto color = to_argb({.r = 220, .g = 227, .b = 234, .a = 255});
        inline constexpr auto dynamic_color = to_argb(Color{.r = 143, .g = 211, .b = 255, .a = 255});
    } // namespace Formula
} // namespace Config
