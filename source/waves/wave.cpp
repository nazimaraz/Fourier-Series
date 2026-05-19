//
// Created by Nazım Can on 25.10.2025.
//

#include "wave.hpp"
#include "math/math.h"
#include "gui/settings.hpp"

using namespace Waves;

Wave::Wave(std::string name, const Type type)
    : name_{std::move(name)}
    , type_{type}
{}

Wave::~Wave() = default;

ComputeResult Wave::compute() const
{
    ComputeResult result;
    result.steps.reserve(settings_->get_number_of_harmonic());
    auto position = raylib::Vector2{0, 0};
    for (auto i = 0u; i < settings_->get_number_of_harmonic(); ++i)
    {
        const auto [n, formula] = get_formula(static_cast<float>(i));
        const auto radius = settings_->get_radius() * formula;
        const auto previous_position = position;
        const auto angle = n * 2 * math::pi_v<float> * settings_->get_frequency() * settings_->get_time();
        position += {radius * math::cos(angle), radius * math::sin(angle)};
        result.steps.emplace_back(previous_position, position, radius);
    }

    result.tip = position;
    return result;
}

void Wave::set_name(std::string name)
{
    name_ = std::move(name);
}

const std::string& Wave::get_name() const
{
    return name_;
}

void Wave::set_type(const Type type)
{
    type_ = type;
}

Type Wave::get_type() const
{
    return type_;
}

void Wave::set_settings(std::shared_ptr<UI::Settings> settings)
{
    settings_ = std::move(settings);
}

const std::shared_ptr<UI::Settings>& Wave::get_settings() const
{
    return settings_;
}
