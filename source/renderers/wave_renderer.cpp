//
// Created by Nazım Can on 20.05.2026.
//

#include <ranges>
#include <raylib.h>
#include "wave_renderer.hpp"
#include "gui/settings.hpp"
#include "waves/wave_variant.hpp"

using namespace Renderers;

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
    };
    const auto result = Waves::compute(selected, params);
    auto& wave = settings_.get_wave();
    auto& path = settings_.get_path();
    if (!settings_.get_is_paused())
    {
        wave.push_front(result.tip.y);
        path.push_front(result.tip);
    }

    auto translate = raylib::Vector2{600, 450};
    if (path.size() > 1)
    {
        points_buffer_.clear();
        points_buffer_.reserve(path.size());
        for (const auto& p : path)
            points_buffer_.emplace_back(translate.x + p.x, translate.y + p.y);

        DrawLineStrip(points_buffer_, raylib::Color{.r = 255, .g = 120, .b = 120, .a = 220});
    }

    for (const auto& step : result.steps)
    {
        DrawCircleLinesV(translate + step.center, step.radius, raylib::Color{.r = 255, .g = 255, .b = 255, .a = 100});
        DrawLineV(translate + step.center, translate + step.tip, raylib::WHITE);
    }

    const auto x_scale = settings_.get_x_scale();
    const auto y_scale = settings_.get_y_scale();
    translate += {200.f, 0.f};
    DrawLineV(translate + result.tip - raylib::Vector2{200.f, 0.f}, translate + raylib::Vector2{0.f, wave.front() * y_scale},
        raylib::WHITE);

    points_buffer_.clear();
    points_buffer_.reserve(wave.size());
    for (const auto i : std::views::iota(size_t{0}, wave.size()))
        points_buffer_.emplace_back(translate.x + static_cast<float>(i) * x_scale, translate.y + wave.at(i) * y_scale);

    DrawLineStrip(points_buffer_, raylib::WHITE);
}
