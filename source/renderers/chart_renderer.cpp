//
// Created by nazim on 5/23/26.
//

#include <array>
#include <cassert>
#include <cstdio>
#include <raylib.h>
#include "chart_renderer.hpp"
#include "config.hpp"
#include "gui/settings.hpp"

using namespace Renderers;
using namespace Config::Chart;

namespace
{
    void format_label(std::array<char, 16>& buf, const char* fmt, const float value)
    {
        const auto written = std::snprintf(buf.data(), buf.size(), fmt, value);
        assert(written >= 0 && static_cast<size_t>(written) < buf.size() && "label formatting failed or was truncated");
    }
} // namespace

ChartRenderer::ChartRenderer(UI::Settings& settings)
    : settings_{settings}
{}

ChartRenderer::~ChartRenderer() = default;

void ChartRenderer::draw() const
{
    const auto origin = Vector2{origin_x, origin_y};
    const auto screen_w = static_cast<float>(GetScreenWidth());
    const auto screen_h = static_cast<float>(GetScreenHeight());
    const auto x_scale = settings_.get_x_scale();
    const auto y_scale = settings_.get_y_scale();
    auto buf = std::array<char, 16>{};
    const auto grid_lines_right = static_cast<int>((screen_w - origin.x) / grid_step_px);
    for (auto i = 1; i <= grid_lines_right; ++i)
    {
        const auto x = origin.x + static_cast<float>(i) * grid_step_px;
        DrawLineV({x, 0.f}, {x, screen_h}, grid_color);
        const auto samples_ago = (x - origin.x) / x_scale;
        format_label(buf, "%.0f", samples_ago);
        DrawText(buf.data(), static_cast<int>(x) + 3, static_cast<int>(origin.y) + 6, label_font, label_color);
        DrawLineV({x, origin.y - tick_len}, {x, origin.y + tick_len}, axis_color);
    }

    const auto grid_lines_left = static_cast<int>((origin.x - chart_left) / grid_step_px);
    for (auto i = 1; i <= grid_lines_left; ++i)
    {
        const auto x = origin.x - static_cast<float>(i) * grid_step_px;
        DrawLineV({x, 0.f}, {x, screen_h}, grid_color);
        DrawLineV({x, origin.y - tick_len}, {x, origin.y + tick_len}, axis_color);
    }

    const auto grid_lines_down = static_cast<int>((screen_h - origin.y) / grid_step_px);
    for (auto i = 1; i <= grid_lines_down; ++i)
    {
        const auto y = origin.y + static_cast<float>(i) * grid_step_px;
        DrawLineV({chart_left, y}, {screen_w, y}, grid_color);
        const auto amp = -(y - origin.y) / y_scale;
        format_label(buf, "%+.0f", amp);
        DrawText(buf.data(), static_cast<int>(origin.x) + 6, static_cast<int>(y) - label_font - 2, label_font, label_color);
        DrawLineV({origin.x - tick_len, y}, {origin.x + tick_len, y}, axis_color);
    }

    const auto grid_lines_up = static_cast<int>(origin.y / grid_step_px);
    for (auto i = 1; i <= grid_lines_up; ++i)
    {
        const auto y = origin.y - static_cast<float>(i) * grid_step_px;
        DrawLineV({chart_left, y}, {screen_w, y}, grid_color);
        const auto amp = -(y - origin.y) / y_scale;
        format_label(buf, "%+.0f", amp);
        DrawText(buf.data(), static_cast<int>(origin.x) + 6, static_cast<int>(y) + 2, label_font, label_color);
        DrawLineV({origin.x - tick_len, y}, {origin.x + tick_len, y}, axis_color);
    }

    DrawLineV({chart_left, origin.y}, {screen_w, origin.y}, axis_color);
    DrawLineV({origin.x, 0.f}, {origin.x, screen_h}, axis_color);
    DrawText("0", static_cast<int>(origin.x) + 4, static_cast<int>(origin.y) + 4, label_font, label_color);
    DrawText("amplitude", static_cast<int>(origin.x) - 60, 6, 12, label_color);
    DrawText("time", static_cast<int>(screen_w) - 36, static_cast<int>(origin.y) + 22, 12, label_color);
}
