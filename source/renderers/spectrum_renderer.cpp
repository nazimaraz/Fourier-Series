//
// Created by nazim on 5/26/26.
//

#include <algorithm>
#include <cstddef>
#include <optional>
#include <ranges>
#include <imgui.h>
#include <raylib.h>
#include "spectrum_renderer.hpp"
#include "chart_renderer.hpp"
#include "gui/settings.hpp"
#include "waves/wave_variant.hpp"

using namespace Renderers;

namespace
{
    constexpr auto enabled_color = raylib::Color{.r = 80, .g = 220, .b = 220, .a = 255};
    constexpr auto disabled_color = raylib::Color{.r = 60, .g = 80, .b = 80, .a = 200};
    constexpr auto frame_color = raylib::Color{.r = 140, .g = 140, .b = 140, .a = 200};
    constexpr auto label_color = raylib::Color{.r = 200, .g = 200, .b = 200, .a = 255};
    constexpr auto title_color = raylib::Color{.r = 150, .g = 150, .b = 150, .a = 230};
    constexpr auto panel_height = 150.f;
    constexpr auto panel_bottom_margin = 30.f;
    constexpr auto panel_right_margin = 10.f;
    constexpr auto title_strip = 18.f;
    constexpr auto bar_gap = 2.f;
    constexpr auto title_font = 12;
} // namespace

SpectrumRenderer::SpectrumRenderer(UI::Settings& settings)
    : settings_{settings}
{}

SpectrumRenderer::~SpectrumRenderer() = default;

void SpectrumRenderer::draw() const
{
    const auto selected = Waves::make_wave_at(settings_.get_selected_wave_index());
    const auto params = Waves::ComputeParams{
        .radius = settings_.get_radius(),
        .harmonic_count = settings_.get_number_of_harmonic(),
        .phase = settings_.get_phase(),
        .enabled_mask = nullptr,
    };
    const auto result = Waves::compute(selected, params);
    if (result.steps.empty())
        return;

    const auto screen_w = static_cast<float>(raylib::GetScreenWidth());
    const auto screen_h = static_cast<float>(raylib::GetScreenHeight());
    constexpr auto panel_left = ChartRenderer::origin_x;
    const auto panel_right = screen_w - panel_right_margin;
    const auto panel_bottom = screen_h - panel_bottom_margin;
    const auto panel_top = panel_bottom - panel_height;
    const auto panel_width = panel_right - panel_left;
    const auto bars_top = panel_top + title_strip;
    const auto bars_height = panel_bottom - bars_top;
    auto max_mag = 0.f;
    for (const auto& step : result.steps)
        max_mag = std::max(max_mag, step.radius);

    if (max_mag == 0.f)
        return;

    const auto num_bars = result.steps.size();
    const auto bar_pitch = panel_width / static_cast<float>(num_bars);
    const auto bar_width = std::max(1.f, bar_pitch - bar_gap);
    for (const auto i : std::views::iota(size_t{0}, num_bars))
    {
        const auto normalized = result.steps[i].radius / max_mag;
        const auto h = normalized * bars_height;
        const auto x = panel_left + static_cast<float>(i) * bar_pitch + bar_gap / 2.f;
        const auto y = panel_bottom - h;
        const auto color = settings_.is_harmonic_enabled(i) ? enabled_color : disabled_color;
        DrawRectangleV({x, y}, {bar_width, h}, color);
    }

    DrawLineV({panel_left, panel_bottom}, {panel_right, panel_bottom}, frame_color);
    DrawLineV({panel_left, bars_top}, {panel_left, panel_bottom}, frame_color);
    DrawText("spectrum", static_cast<int>(panel_left) + 4, static_cast<int>(panel_top) + 2, title_font, label_color);
    DrawText("(click: toggle | right-click: solo)", static_cast<int>(panel_left) + 70, static_cast<int>(panel_top) + 3, 10,
        title_color);
    if (ImGui::GetIO().WantCaptureMouse)
        return;

    const auto column_under_mouse = [&]() -> std::optional<size_t> {
        const auto mouse = raylib::GetMousePosition();
        if (mouse.x < panel_left || mouse.x >= panel_right)
            return std::nullopt;

        if (mouse.y < bars_top || mouse.y >= panel_bottom)
            return std::nullopt;

        const auto col = static_cast<size_t>((mouse.x - panel_left) / bar_pitch);
        if (col >= num_bars)
            return std::nullopt;

        return col;
    };

    if (raylib::IsMouseButtonPressed(raylib::MouseButton::MOUSE_BUTTON_LEFT))
    {
        if (const auto col = column_under_mouse())
            settings_.toggle_harmonic(*col);
    }
    else if (raylib::IsMouseButtonPressed(raylib::MouseButton::MOUSE_BUTTON_RIGHT))
    {
        if (const auto col = column_under_mouse())
            settings_.solo_harmonic(*col);
    }
}
