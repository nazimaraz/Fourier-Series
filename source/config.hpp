//
// Created by nazim on 5/26/26.
//

#pragma once

#include <raylib/color.h>

namespace Config
{
    namespace Chart
    {
        inline constexpr auto origin_x = 800.f;
        inline constexpr auto origin_y = 450.f;
        inline constexpr auto chart_left = 200.f;
        inline constexpr auto grid_step_px = 80.f;
        inline constexpr auto tick_len = 4.f;
        inline constexpr auto axis_color = raylib::Color{.r = 140, .g = 140, .b = 140, .a = 200};
        inline constexpr auto grid_color = raylib::Color{.r = 55, .g = 55, .b = 55, .a = 130};
        inline constexpr auto label_color = raylib::Color{.r = 190, .g = 190, .b = 190, .a = 255};
        inline constexpr auto label_font = 11;
    } // namespace Chart

    namespace Wave
    {
        inline constexpr auto epicycle_origin_x = 600.f;
        inline constexpr auto epicycle_origin_y = 450.f;
        inline constexpr auto substeps_per_cycle = 1000.f;
        inline constexpr auto substeps_max = 64;
        inline constexpr auto wave_color = raylib::Color{.r = 255, .g = 60, .b = 60, .a = 255};
        inline constexpr auto epicycle_color = raylib::WHITE;
        inline constexpr auto circle_color = raylib::Color{.r = 255, .g = 255, .b = 255, .a = 100};
        inline constexpr auto path_color = raylib::Color{.r = 255, .g = 140, .b = 140, .a = 200};
    } // namespace Wave

    namespace Spectrum
    {
        inline constexpr auto panel_height = 150.f;
        inline constexpr auto panel_bottom_margin = 30.f;
        inline constexpr auto panel_right_margin = 10.f;
        inline constexpr auto title_strip = 18.f;
        inline constexpr auto bar_gap = 2.f;
        inline constexpr auto enabled_color = raylib::Color{.r = 80, .g = 220, .b = 220, .a = 255};
        inline constexpr auto disabled_color = raylib::Color{.r = 60, .g = 80, .b = 80, .a = 200};
        inline constexpr auto frame_color = raylib::Color{.r = 140, .g = 140, .b = 140, .a = 200};
        inline constexpr auto label_color = raylib::Color{.r = 200, .g = 200, .b = 200, .a = 255};
        inline constexpr auto title_color = raylib::Color{.r = 150, .g = 150, .b = 150, .a = 230};
        inline constexpr auto title_font = 12;
        inline constexpr auto hint_font = 10;
    } // namespace Spectrum
} // namespace Config
