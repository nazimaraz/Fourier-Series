//
// Created by Nazım Can on 25.10.2025.
//

#include "settings.hpp"
#include "waves/wave.hpp"

using namespace UI;

void Settings::set_fps(const int fps)
{
    fps_ = fps;
}

int Settings::get_fps() const
{
    return fps_;
}

void Settings::set_background_color(const raylib::Color color)
{
    background_color_ = color;
}

raylib::Color Settings::get_background_color() const
{
    return background_color_;
}

void Settings::set_wave_capacity(const boost::circular_buffer<float>::capacity_type capacity)
{
    wave_.set_capacity(capacity);
}

boost::circular_buffer<float>& Settings::get_wave()
{
    return wave_;
}

void Settings::set_is_paused(const bool is_paused)
{
    is_paused_ = is_paused;
}

[[nodiscard]] bool Settings::get_is_paused() const
{
    return is_paused_;
}

void Settings::add_wave(const std::shared_ptr<Waves::Wave>& wave)
{
    wave->set_settings(shared_from_this());
    waves_.try_emplace(wave->get_type(), wave);
    wave_name_waves_.try_emplace(wave->get_name(), wave);
    wave_names_.push_back(wave->get_name().c_str());
    wave_names_types_.insert({wave->get_type(), wave->get_name()});
}

const std::shared_ptr<Waves::Wave>& Settings::get_wave(const std::string& name)
{
    return wave_name_waves_.at(name);
}

const std::shared_ptr<Waves::Wave>& Settings::get_wave(const Waves::Type type)
{
    return waves_.at(type);
}

const std::vector<const char*>& Settings::get_wave_names() const
{
    return wave_names_;
}

const std::string& Settings::get_wave_name(const Waves::Type type) const
{
    return wave_names_types_.left.at(type);
}

Waves::Type Settings::get_wave_type(const std::string& name) const
{
    return wave_names_types_.right.at(name);
}

void Settings::set_selected_wave(const std::shared_ptr<Waves::Wave>& wave)
{
    selected_wave_type_ = wave->get_type();
}

void Settings::set_selected_wave(const Waves::Type type)
{
    selected_wave_type_ = type;
}

void Settings::set_selected_wave(const std::string& name)
{
    set_selected_wave(get_wave(name));
}

Waves::Type Settings::get_selected_wave_type() const
{
    return selected_wave_type_;
}

const std::shared_ptr<Waves::Wave>& Settings::get_selected_wave() const
{
    return waves_.at(selected_wave_type_);
}

void Settings::set_number_of_harmonic(const unsigned int number)
{
    number_of_harmonic_ = number;
}

unsigned int& Settings::get_number_of_harmonic()
{
    return number_of_harmonic_;
}

void Settings::set_frequency(const float frequency)
{
    frequency_ = frequency;
}

float& Settings::get_frequency()
{
    return frequency_;
}

void Settings::set_radius(const float radius)
{
    radius_ = radius;
}

float& Settings::get_radius()
{
    return radius_;
}

void Settings::set_phase(const float phase)
{
    phase_ = phase;
}

float Settings::get_phase() const
{
    return phase_;
}

void Settings::set_x_scale(const float x_scale)
{
    x_scale_ = x_scale;
}

float& Settings::get_x_scale()
{
    return x_scale_;
}

void Settings::set_y_scale(const float y_scale)
{
    y_scale_ = y_scale;
}

float& Settings::get_y_scale()
{
    return y_scale_;
}
