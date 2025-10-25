//
// Created by Nazım Can on 25.10.2025.
//

#include "wave.hpp"
#include <utility>
#include "raylib.h"
#include "vector2.hpp"
#include "math/math.h"

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
        const auto [n, radius] = get_formula(static_cast<float>(i));
        DrawCircleLinesV(translate + position, radius, raylib::Color{255, 255, 255, 100});
        const auto previous_position = position;
        position += {
            radius * math::cos(n * settings_->get_time()),
            radius * math::sin(n * settings_->get_time())
        };
        DrawLineV(translate + previous_position, translate + position, raylib::RAYWHITE);
    }

    auto& wave = settings_->get_wave();
    if (!settings_->get_is_paused())
        wave.push_front(position.y);

    translate += {200, 0};
    DrawLineV(translate + position - raylib::Vector2{200, 0}, translate + raylib::Vector2{0, wave.front()}, raylib::RAYWHITE);
    std::vector<raylib::Vector2> points;
    points.reserve(wave.size());
    for (auto i = 0; i < wave.size(); ++i)
        points.emplace_back(translate.x + static_cast<float>(i), translate.y + wave.at(i));

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
