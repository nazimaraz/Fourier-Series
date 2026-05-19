//
// Created by Nazım Can on 20.05.2026.
//

#include <raylib.h>
#include "wave_renderer.hpp"
#include "gui/settings.hpp"
#include "waves/wave.hpp"

using namespace Renderers;

WaveRenderer::WaveRenderer(std::shared_ptr<UI::Settings> settings)
    : settings_{std::move(settings)}
{}

void WaveRenderer::draw() const
{
    const auto result = settings_->get_selected_wave()->compute();
    auto& wave = settings_->get_wave();
    if (!settings_->get_is_paused())
        wave.push_front(result.tip.y);

    auto translate = raylib::Vector2{600, 450};
    for (const auto& step : result.steps)
    {
        DrawCircleLinesV(translate + step.center, step.radius, raylib::Color{255, 255, 255, 100});
        DrawLineV(translate + step.center, translate + step.tip, raylib::WHITE);
    }

    const auto x_scale = settings_->get_x_scale();
    const auto y_scale = settings_->get_y_scale();
    translate += {200.f, 0.f};
    DrawLineV(translate + result.tip - raylib::Vector2{200.f, 0.f}, translate + raylib::Vector2{0.f, wave.front() * y_scale},
        raylib::WHITE);

    auto points = std::vector<raylib::Vector2>{};
    points.reserve(wave.size());
    for (auto i = 0; i < wave.size(); ++i)
        points.emplace_back(translate.x + static_cast<float>(i) * x_scale, translate.y + wave.at(i) * y_scale);

    DrawLineStrip(points, raylib::WHITE);
}
