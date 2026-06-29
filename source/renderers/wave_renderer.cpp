//
// Created by Nazım Can on 20.05.2026.
//

#include <algorithm>
#include <cstddef>
#include <ranges>
#include <span>
#include <raylib.h>
#include "wave_renderer.hpp"
#include "config.hpp"
#include "raymath.h"
#include "gui/settings.hpp"
#include "waves/wave_variant.hpp"

using namespace Renderers;

namespace
{
    auto draw_polyline(const std::span<const Vector2> points, const Color color, const float thickness) -> void
    {
        if (points.size() < 2)
            return;

        for (auto i = std::size_t{1}; i < points.size(); ++i)
            DrawLineEx(points[i - 1], points[i], thickness, color);
    }
} // namespace

WaveRenderer::WaveRenderer(UI::Settings& settings)
    : settings_{settings}
{}

WaveRenderer::~WaveRenderer() = default;

auto WaveRenderer::draw() const -> void
{
    const auto& selected = settings_.get_selected_wave();
    const auto params = Waves::ComputeParams{
        .radius = settings_.radius(),
        .harmonic_count = settings_.number_of_harmonic(),
        .phase = settings_.get_phase(),
        .enabled_mask = settings_.get_harmonic_mask(),
    };
    const auto result = Waves::compute(selected, params);
    auto& wave = settings_.wave();
    auto& path = settings_.path();
    if (!settings_.get_is_paused())
    {
        wave.push_front(result.tip.y);
        if (initialized_)
        {
            if (const auto phase_delta = params.phase - last_phase_; phase_delta > 0.f)
            {
                const auto substeps =
                    std::clamp(static_cast<int>(phase_delta * Config::Wave::substeps_per_cycle), 1, Config::Wave::substeps_max);
                for (auto k = 1; k < substeps; ++k)
                {
                    auto sub_params = params;
                    sub_params.phase = last_phase_ + phase_delta * static_cast<float>(k) / static_cast<float>(substeps);
                    const auto [steps, tip] = Waves::compute(selected, sub_params);
                    path.push_front(tip);
                }
            }
        }

        path.push_front(result.tip);
    }

    last_phase_ = params.phase;
    initialized_ = true;
    auto translate = Vector2{Config::Wave::epicycle_origin_x, Config::Wave::epicycle_origin_y};
    const auto math_to_screen = [&translate](const Vector2 p) -> Vector2 {
        return Vector2{translate.x + p.x, translate.y - p.y};
    };
    for (const auto& step : result.steps)
    {
        DrawCircleLinesV(math_to_screen(step.center), step.radius, Config::Wave::circle_color);
        DrawLineEx(math_to_screen(step.center), math_to_screen(step.tip), Config::Wave::epicycle_thickness,
            Config::Wave::epicycle_color);
    }

    if (path.size() > 1)
    {
        points_buffer_.clear();
        points_buffer_.reserve(path.size());
        for (const auto& p : path)
            points_buffer_.push_back(math_to_screen(p));

        draw_polyline(points_buffer_, Config::Wave::path_color, Config::Wave::path_thickness);
    }

    if (const auto& drawing = settings_.get_drawing_points(); drawing.size() > 1)
    {
        points_buffer_.clear();
        points_buffer_.reserve(drawing.size());
        for (const auto& p : drawing)
            points_buffer_.push_back(math_to_screen(p));

        draw_polyline(points_buffer_, Config::Wave::drawing_color, Config::Wave::drawing_thickness);
    }

    const auto x_scale = settings_.x_scale();
    const auto y_scale = settings_.y_scale();
    translate = Vector2{Config::Chart::origin_x, Config::Chart::origin_y};
    constexpr auto epicycle_offset = Config::Chart::origin_x - Config::Wave::epicycle_origin_x;
    const auto epicycle_tip_on_screen = Vector2Subtract(math_to_screen(result.tip), {epicycle_offset, 0.f});
    const auto wave_start_on_screen = math_to_screen({0.f, wave.front() * y_scale});
    DrawLineEx(epicycle_tip_on_screen, wave_start_on_screen, Config::Wave::connector_thickness, Config::Wave::connector_color);
    points_buffer_.clear();
    points_buffer_.reserve(wave.size());
    for (const auto i : std::views::iota(std::size_t{0}, wave.size()))
        points_buffer_.push_back(math_to_screen({static_cast<float>(i) * x_scale, wave.at(i) * y_scale}));

    draw_polyline(points_buffer_, Config::Wave::wave_color, Config::Wave::wave_thickness);
}
