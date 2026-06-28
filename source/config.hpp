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
        inline constexpr auto background_color = Color{0, 0, 0, 255};
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
        inline constexpr auto axis_color = Color{.r = 140, .g = 140, .b = 140, .a = 200};
        inline constexpr auto grid_color = Color{.r = 55, .g = 55, .b = 55, .a = 130};
        inline constexpr auto label_color = Color{.r = 190, .g = 190, .b = 190, .a = 255};
        inline constexpr auto label_font = 11;
    } // namespace Chart

    namespace Wave
    {
        inline constexpr auto epicycle_origin_x = 600.f;
        inline constexpr auto epicycle_origin_y = 450.f;
        inline constexpr auto substeps_per_cycle = 1000.f;
        inline constexpr auto substeps_max = 64;
        inline constexpr auto wave_color = Color{.r = 255, .g = 60, .b = 60, .a = 255};
        inline constexpr auto epicycle_color = Color{255, 255, 255, 255};
        inline constexpr auto circle_color = Color{.r = 255, .g = 255, .b = 255, .a = 100};
        inline constexpr auto path_color = Color{.r = 255, .g = 140, .b = 140, .a = 200};
    } // namespace Wave

    namespace Spectrum
    {
        inline constexpr auto panel_height = 150.f;
        inline constexpr auto panel_bottom_margin = 30.f;
        inline constexpr auto panel_right_margin = 10.f;
        inline constexpr auto title_strip = 18.f;
        inline constexpr auto bar_gap = 2.f;
        inline constexpr auto enabled_color = Color{.r = 80, .g = 220, .b = 220, .a = 255};
        inline constexpr auto disabled_color = Color{.r = 60, .g = 80, .b = 80, .a = 200};
        inline constexpr auto frame_color = Color{.r = 140, .g = 140, .b = 140, .a = 200};
        inline constexpr auto label_color = Color{.r = 200, .g = 200, .b = 200, .a = 255};
        inline constexpr auto title_color = Color{.r = 150, .g = 150, .b = 150, .a = 230};
        inline constexpr auto title_font = 12;
        inline constexpr auto hint_font = 10;
    } // namespace Spectrum

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
        inline constexpr auto vertical_gap = 6.f;
        inline constexpr auto color = to_argb({.r = 190, .g = 190, .b = 190, .a = 255});
        inline constexpr auto dynamic_color = to_argb(Color{.r = 154, .g = 209, .b = 255, .a = 255});
    } // namespace Formula
} // namespace Config
