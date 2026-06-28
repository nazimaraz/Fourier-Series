//
// Created by nazim on 5/23/26.
//

#include <iomanip>
#include <raylib.h>
#include <sstream>
#include <string>
#include "chart_renderer.hpp"
#include "config.hpp"
#include "gui/settings.hpp"

using namespace Renderers;
using namespace Config::Chart;

namespace
{
    [[nodiscard]] auto format_label(const float value, const bool show_sign = false) -> std::string
    {
        auto stream = std::ostringstream{};
        if (show_sign)
            stream << std::showpos;

        stream << std::fixed << std::setprecision(0) << value;
        return stream.str();
    }
} // namespace

ChartRenderer::ChartRenderer(UI::Settings& settings)
    : settings_{settings}
{}

auto ChartRenderer::draw() const -> void
{
    constexpr auto origin = Vector2{.x = origin_x, .y = origin_y};
    const auto screen_w = static_cast<float>(GetScreenWidth());
    const auto screen_h = static_cast<float>(GetScreenHeight());
    const auto x_scale = settings_.get_x_scale();
    const auto y_scale = settings_.get_y_scale();
    const auto grid_lines_right = static_cast<int>((screen_w - origin.x) / grid_step_px);
    for (auto i = 1; i <= grid_lines_right; ++i)
    {
        const auto x = origin.x + static_cast<float>(i) * grid_step_px;
        DrawLineV({.x = x, .y = 0.f}, {.x = x, .y = screen_h}, grid_color);
        const auto samples_ago = (x - origin.x) / x_scale;
        const auto label = format_label(samples_ago);
        DrawText(label.c_str(), static_cast<int>(x) + 3, static_cast<int>(origin.y) + 6, label_font, label_color);
        DrawLineV({x, origin.y - tick_len}, {x, origin.y + tick_len}, axis_color);
    }

    constexpr auto grid_lines_left = static_cast<int>((origin.x - chart_left) / grid_step_px);
    for (auto i = 1; i <= grid_lines_left; ++i)
    {
        const auto x = origin.x - static_cast<float>(i) * grid_step_px;
        DrawLineV({.x = x, .y = 0.f}, {.x = x, .y = screen_h}, grid_color);
        DrawLineV({.x = x, .y = origin.y - tick_len}, {.x = x, .y = origin.y + tick_len}, axis_color);
    }

    const auto grid_lines_down = static_cast<int>((screen_h - origin.y) / grid_step_px);
    for (auto i = 1; i <= grid_lines_down; ++i)
    {
        const auto y = origin.y + static_cast<float>(i) * grid_step_px;
        DrawLineV({chart_left, y}, {screen_w, y}, grid_color);
        const auto amp = -(y - origin.y) / y_scale;
        const auto label = format_label(amp, true);
        DrawText(label.c_str(), static_cast<int>(origin.x) + 6, static_cast<int>(y) - label_font - 2, label_font, label_color);
        DrawLineV({origin.x - tick_len, y}, {origin.x + tick_len, y}, axis_color);
    }

    constexpr auto grid_lines_up = static_cast<int>(origin.y / grid_step_px);
    for (auto i = 1; i <= grid_lines_up; ++i)
    {
        const auto y = origin.y - static_cast<float>(i) * grid_step_px;
        DrawLineV({chart_left, y}, {screen_w, y}, grid_color);
        const auto amp = -(y - origin.y) / y_scale;
        const auto label = format_label(amp, true);
        DrawText(label.c_str(), static_cast<int>(origin.x) + 6, static_cast<int>(y) + 2, label_font, label_color);
        DrawLineV({origin.x - tick_len, y}, {origin.x + tick_len, y}, axis_color);
    }

    DrawLineV({chart_left, origin.y}, {screen_w, origin.y}, axis_color);
    DrawLineV({origin.x, 0.f}, {origin.x, screen_h}, axis_color);
    DrawText("0", static_cast<int>(origin.x) + 4, static_cast<int>(origin.y) + 4, label_font, label_color);
    DrawText("amplitude", static_cast<int>(origin.x) - 60, 6, 12, label_color);
    DrawText("time", static_cast<int>(screen_w) - 36, static_cast<int>(origin.y) + 22, 12, label_color);
}
