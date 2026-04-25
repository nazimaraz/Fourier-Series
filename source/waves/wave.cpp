//
// Created by Nazım Can on 25.10.2025.
//

#include "wave.hpp"
#include <utility>
#include "raylib.h"
#include "vector2.hpp"
#include "math/math.h"
#include "gui/settings.hpp"

using namespace Waves;

Wave::Wave(std::string name, const Type type)
    : name_{std::move(name)}
    , type_{type}
{}

Wave::~Wave() = default;

void Wave::update() const
{
    auto translate = raylib::Vector2{600, 450};
    auto position = raylib::Vector2{0, 0};
    for (auto i = 0u; i < settings_->get_number_of_harmonic(); ++i)
    {
        const auto [n, formula] = get_formula(static_cast<float>(i));
        const auto radius = settings_->get_radius() * formula;
        DrawCircleLinesV(translate + position, radius, raylib::Color{255, 255, 255, 100});
        const auto previous_position = position;
        const auto angle = n * 2 * math::pi_v<float> * settings_->get_frequency() * settings_->get_time();
        position += {
            radius * math::cos(angle),
            radius * math::sin(angle)
        };
        DrawLineV(translate + previous_position, translate + position, raylib::WHITE);
    }

    auto& wave = settings_->get_wave();
    if (!settings_->get_is_paused())
        wave.push_front(position.y);

    const auto x_scale = settings_->get_x_scale();
    const auto y_scale = settings_->get_y_scale();
    translate += {200, 0};
    DrawLineV(translate + position - raylib::Vector2{200, 0}, translate + raylib::Vector2{0, wave.front() * y_scale}, raylib::WHITE);
    std::vector<raylib::Vector2> points;
    points.reserve(wave.size());
    for (auto i = 0; i < wave.size(); ++i)
        points.emplace_back(translate.x + static_cast<float>(i) * x_scale, translate.y + wave.at(i) * y_scale);

    DrawLineStrip(points, raylib::WHITE);
}

void Wave::set_name(const std::string& name)
{
    name_ = name;
}

[[nodiscard]] const std::string& Wave::get_name() const
{
    return name_;
}

void Wave::set_type(const Type type)
{
    type_ = type;
}

[[nodiscard]] Type Wave::get_type() const
{
    return type_;
}

void Wave::set_settings(const std::shared_ptr<UI::Settings>& settings)
{
    settings_ = settings;
}

const std::shared_ptr<UI::Settings>& Wave::get_settings() const
{
    return settings_;
}
