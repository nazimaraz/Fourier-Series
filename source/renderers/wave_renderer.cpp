//
// Created by Nazım Can on 20.05.2026.
//

#include <ranges>
#include <raylib.h>
#include "chart_renderer.hpp"
#include "wave_renderer.hpp"
#include "gui/settings.hpp"
#include "waves/wave_variant.hpp"

using namespace Renderers;

namespace
{
    constexpr auto wave_color = raylib::Color{.r = 255, .g = 60, .b = 60, .a = 255};
    constexpr auto epicycle_color = raylib::WHITE;
    constexpr auto circle_color = raylib::Color{.r = 255, .g = 255, .b = 255, .a = 100};
    constexpr auto path_color = raylib::Color{.r = 255, .g = 140, .b = 140, .a = 200};
} // namespace

WaveRenderer::WaveRenderer(UI::Settings& settings)
    : settings_{settings}
{}

WaveRenderer::~WaveRenderer() = default;

void WaveRenderer::draw() const
{
    const auto selected = Waves::make_wave_at(settings_.get_selected_wave_index());
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
                const auto substeps = std::clamp(static_cast<int>(phase_delta * 1000.f), 1, 64);
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

    auto translate = raylib::Vector2{600, 450};
    const auto math_to_screen = [&translate](const raylib::Vector2 p) {
        return raylib::Vector2{translate.x + p.x, translate.y - p.y};
    };
    if (path.size() > 1)
    {
        points_buffer_.clear();
        points_buffer_.reserve(path.size());
        for (const auto& p : path)
            points_buffer_.push_back(math_to_screen(p));

        DrawLineStrip(points_buffer_, path_color);
    }

    for (const auto& step : result.steps)
    {
        DrawCircleLinesV(math_to_screen(step.center), step.radius, circle_color);
        DrawLineV(math_to_screen(step.center), math_to_screen(step.tip), epicycle_color);
    }

    const auto x_scale = settings_.get_x_scale();
    const auto y_scale = settings_.get_y_scale();
    translate = raylib::Vector2{ChartRenderer::origin_x, ChartRenderer::origin_y};
    const auto epicycle_tip_on_screen = math_to_screen(result.tip) - raylib::Vector2{200.f, 0.f};
    const auto wave_start_on_screen = math_to_screen({0.f, wave.front() * y_scale});
    DrawLineV(epicycle_tip_on_screen, wave_start_on_screen, wave_color);
    points_buffer_.clear();
    points_buffer_.reserve(wave.size());
    for (const auto i : std::views::iota(size_t{0}, wave.size()))
        points_buffer_.push_back(math_to_screen({static_cast<float>(i) * x_scale, wave.at(i) * y_scale}));

    DrawLineStrip(points_buffer_, wave_color);
}
