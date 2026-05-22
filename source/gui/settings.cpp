//
// Created by Nazım Can on 25.10.2025.
//

#include "settings.hpp"

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

void Settings::set_path_capacity(const boost::circular_buffer<raylib::Vector2>::capacity_type capacity)
{
    path_.set_capacity(capacity);
}

boost::circular_buffer<raylib::Vector2>& Settings::get_path()
{
    return path_;
}

void Settings::set_is_paused(const bool is_paused)
{
    is_paused_ = is_paused;
}

bool Settings::get_is_paused() const
{
    return is_paused_;
}

void Settings::set_selected_wave_index(const size_t index)
{
    selected_wave_index_ = index;
}

size_t Settings::get_selected_wave_index() const
{
    return selected_wave_index_;
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
