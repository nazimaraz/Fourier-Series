//
// Created by Nazım Can on 25.10.2025.
//

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <utility>
#include "settings.hpp"

using namespace UI;

Settings::Settings()
{
    reset_harmonic_mask();
}

auto Settings::is_harmonic_enabled(const std::size_t i) const -> bool
{
    return harmonic_enabled_.at(i);
}

auto Settings::toggle_harmonic(const std::size_t i) -> void
{
    harmonic_enabled_.at(i) = !harmonic_enabled_.at(i);
}

auto Settings::is_only_harmonic_enabled(const std::size_t i) const -> bool
{
    return is_harmonic_enabled(i) && std::ranges::count(harmonic_enabled_, true) == 1;
}

auto Settings::solo_harmonic(const std::size_t i) -> void
{
    if (is_only_harmonic_enabled(i))
    {
        reset_harmonic_mask();
        return;
    }

    harmonic_enabled_.fill(false);
    harmonic_enabled_.at(i) = true;
}

auto Settings::reset_harmonic_mask() -> void
{
    harmonic_enabled_.fill(true);
}

auto Settings::get_harmonic_mask() const -> const std::array<bool, Config::Defaults::max_harmonic_count>&
{
    return harmonic_enabled_;
}

auto Settings::set_fps(const int fps) -> void
{
    fps_ = fps;
}

auto Settings::get_fps() const -> int
{
    return fps_;
}

auto Settings::set_background_color(const Color color) -> void
{
    background_color_ = color;
}

auto Settings::get_background_color() const -> Color
{
    return background_color_;
}

auto Settings::set_wave_capacity(const boost::circular_buffer<float>::capacity_type capacity) -> void
{
    wave_.set_capacity(capacity);
}

auto Settings::wave() -> boost::circular_buffer<float>&
{
    return wave_;
}

auto Settings::set_path_capacity(const boost::circular_buffer<Vector2>::capacity_type capacity) -> void
{
    path_.set_capacity(capacity);
}

auto Settings::path() -> boost::circular_buffer<Vector2>&
{
    return path_;
}

auto Settings::set_is_paused(const bool is_paused) -> void
{
    is_paused_ = is_paused;
}

auto Settings::get_is_paused() const -> bool
{
    return is_paused_;
}

auto Settings::set_selected_wave_index(const std::size_t index) -> void
{
    if (selected_wave_.index() == index)
        return;

    selected_wave_ = Waves::make_wave_at(index);
    drawing_points_.clear();
    is_drawing_ = false;
}

auto Settings::get_selected_wave_index() const -> std::size_t
{
    return selected_wave_.index();
}

auto Settings::get_selected_wave() const -> const Waves::WaveVariant&
{
    return selected_wave_;
}

auto Settings::get_selected_wave() -> Waves::WaveVariant&
{
    return selected_wave_;
}

auto Settings::start_drawing() -> void
{
    drawing_points_.clear();
    is_drawing_ = true;
}

auto Settings::stop_drawing() -> void
{
    is_drawing_ = false;
}

auto Settings::add_drawing_point(const Vector2 p) -> void
{
    if (!drawing_points_.empty())
    {
        if (const auto& last = drawing_points_.back(); std::hypot(p.x - last.x, p.y - last.y) < 1.f)
            return;
    }

    drawing_points_.push_back(p);
}

auto Settings::set_drawing_points(std::vector<Vector2> points) -> void
{
    drawing_points_ = std::move(points);
}

auto Settings::set_selected_wave(Waves::WaveVariant wave) -> void
{
    selected_wave_ = std::move(wave);
}

auto Settings::is_drawing() const -> bool
{
    return is_drawing_;
}

auto Settings::get_drawing_points() const -> const std::vector<Vector2>&
{
    return drawing_points_;
}

auto Settings::set_number_of_harmonic(const unsigned int number) -> void
{
    number_of_harmonic_ = number;
}

auto Settings::number_of_harmonic() const -> unsigned int
{
    return number_of_harmonic_;
}

auto Settings::mutable_number_of_harmonic() -> unsigned int&
{
    return number_of_harmonic_;
}

auto Settings::set_frequency(const float frequency) -> void
{
    frequency_ = frequency;
}

auto Settings::frequency() const -> float
{
    return frequency_;
}

auto Settings::mutable_frequency() -> float&
{
    return frequency_;
}

auto Settings::set_radius(const float radius) -> void
{
    radius_ = radius;
}

auto Settings::radius() const -> float
{
    return radius_;
}

auto Settings::mutable_radius() -> float&
{
    return radius_;
}

auto Settings::set_phase(const float phase) -> void
{
    phase_ = phase;
}

auto Settings::get_phase() const -> float
{
    return phase_;
}

auto Settings::set_x_scale(const float x_scale) -> void
{
    x_scale_ = x_scale;
}

auto Settings::x_scale() const -> float
{
    return x_scale_;
}

auto Settings::mutable_x_scale() -> float&
{
    return x_scale_;
}

auto Settings::set_y_scale(const float y_scale) -> void
{
    y_scale_ = y_scale;
}

auto Settings::y_scale() const -> float
{
    return y_scale_;
}

auto Settings::mutable_y_scale() -> float&
{
    return y_scale_;
}
