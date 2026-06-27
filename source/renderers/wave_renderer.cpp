//
// Created by Nazım Can on 20.05.2026.
//

#include <algorithm>
#include <ranges>
#include <raylib.h>
#include "wave_renderer.hpp"
#include "config.hpp"
#include "raymath.h"
#include "gui/settings.hpp"
#include "waves/wave_variant.hpp"

using namespace Renderers;

WaveRenderer::WaveRenderer(UI::Settings& settings)
    : settings_{settings}
{}

WaveRenderer::~WaveRenderer() = default;

void WaveRenderer::draw() const
{
    const auto& selected = settings_.get_selected_wave();
    const auto params = Waves::ComputeParams{
        .radius = settings_.get_radius(),
        .harmonic_count = settings_.get_number_of_harmonic(),
        .phase = settings_.get_phase(),
        .enabled_mask = settings_.get_harmonic_mask().data(),
    };
    const auto result = Waves::compute(selected, params);
    auto& wave = settings_.get_wave();
    auto& path = settings_.get_path();
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
    const auto math_to_screen = [&translate](const Vector2 p) {
        return Vector2{translate.x + p.x, translate.y - p.y};
    };
    if (path.size() > 1)
    {
        points_buffer_.clear();
        points_buffer_.reserve(path.size());
        for (const auto& p : path)
            points_buffer_.push_back(math_to_screen(p));

        DrawLineStrip(points_buffer_.data(), static_cast<int>(points_buffer_.size()), Config::Wave::path_color);
    }

    if (const auto& drawing = settings_.get_drawing_points(); drawing.size() > 1)
    {
        points_buffer_.clear();
        points_buffer_.reserve(drawing.size());
        for (const auto& p : drawing)
            points_buffer_.push_back(math_to_screen(p));

        constexpr auto draw_color = Color{.r = 200, .g = 200, .b = 200, .a = 180};
        DrawLineStrip(points_buffer_.data(), static_cast<int>(points_buffer_.size()), draw_color);
    }

    for (const auto& step : result.steps)
    {
        DrawCircleLinesV(math_to_screen(step.center), step.radius, Config::Wave::circle_color);
        DrawLineV(math_to_screen(step.center), math_to_screen(step.tip), Config::Wave::epicycle_color);
    }

    const auto x_scale = settings_.get_x_scale();
    const auto y_scale = settings_.get_y_scale();
    translate = Vector2{Config::Chart::origin_x, Config::Chart::origin_y};
    constexpr auto epicycle_offset = Config::Chart::origin_x - Config::Wave::epicycle_origin_x;
    const auto epicycle_tip_on_screen = Vector2Subtract(math_to_screen(result.tip), {epicycle_offset, 0.f});
    const auto wave_start_on_screen = math_to_screen({0.f, wave.front() * y_scale});
    DrawLineV(epicycle_tip_on_screen, wave_start_on_screen, Config::Wave::wave_color);
    points_buffer_.clear();
    points_buffer_.reserve(wave.size());
    for (const auto i : std::views::iota(size_t{0}, wave.size()))
        points_buffer_.push_back(math_to_screen({static_cast<float>(i) * x_scale, wave.at(i) * y_scale}));

    DrawLineStrip(points_buffer_.data(), static_cast<int>(points_buffer_.size()), Config::Wave::wave_color);
}
