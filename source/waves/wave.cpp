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
    const auto base_radius = settings_->get_radius();
    auto position = raylib::Vector2{0.f, base_radius * get_dc()};
    for (auto i = 0u; i < settings_->get_number_of_harmonic(); ++i)
    {
        const auto [n, coefficient, phase] = get_formula(static_cast<float>(i));
        const auto radius = base_radius * coefficient;
        const auto previous_position = position;
        const auto angle = n * 2.f * math::pi_v<float> * settings_->get_phase() + phase;
        position += {radius * math::cos(angle), radius * math::sin(angle)};
        result.steps.emplace_back(previous_position, position, math::abs(radius));
    }

    result.tip = position;
    return result;
}

float Wave::get_dc() const
{
    return 0.f;
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
