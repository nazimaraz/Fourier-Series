//
// Created by nazim on 5/26/26.
//

#include <algorithm>
#include <cstddef>
#include <ranges>
#include <raylib.h>
#include "spectrum_renderer.hpp"
#include "chart_renderer.hpp"
#include "gui/settings.hpp"
#include "waves/wave_variant.hpp"

using namespace Renderers;

namespace
{
    constexpr auto bar_color = raylib::Color{.r = 80, .g = 220, .b = 220, .a = 255};
    constexpr auto frame_color = raylib::Color{.r = 140, .g = 140, .b = 140, .a = 200};
    constexpr auto label_color = raylib::Color{.r = 200, .g = 200, .b = 200, .a = 255};
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
        DrawRectangleV({x, y}, {bar_width, h}, bar_color);
    }

    DrawLineV({panel_left, panel_bottom}, {panel_right, panel_bottom}, frame_color);
    DrawLineV({panel_left, bars_top}, {panel_left, panel_bottom}, frame_color);
    DrawText("spectrum", static_cast<int>(panel_left) + 4, static_cast<int>(panel_top) + 2, title_font, label_color);
}
