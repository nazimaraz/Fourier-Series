//
// Created by Nazım Can on 25.10.2025.
//

#pragma once

#include <array>
#include <boost/circular_buffer.hpp>
#include <raylib/color.h>
#include "vector2.hpp"

namespace UI
{
    class Settings
    {
    public:
        static constexpr size_t max_harmonic_count = 100;

        Settings();
        void set_fps(int fps);
        [[nodiscard]] int get_fps() const;
        void set_background_color(raylib::Color color);
        [[nodiscard]] raylib::Color get_background_color() const;
        void set_wave_capacity(boost::circular_buffer<float>::capacity_type capacity);
        [[nodiscard]] boost::circular_buffer<float>& get_wave();
        void set_path_capacity(boost::circular_buffer<raylib::Vector2>::capacity_type capacity);
        [[nodiscard]] boost::circular_buffer<raylib::Vector2>& get_path();
        void set_is_paused(bool is_paused);
        [[nodiscard]] bool get_is_paused() const;
        void set_selected_wave_index(size_t index);
        [[nodiscard]] size_t get_selected_wave_index() const;
        void set_number_of_harmonic(unsigned int number);
        [[nodiscard]] unsigned int& get_number_of_harmonic();
        void set_frequency(float frequency);
        [[nodiscard]] float& get_frequency();
        void set_radius(float radius);
        [[nodiscard]] float& get_radius();
        void set_phase(float phase);
        [[nodiscard]] float get_phase() const;
        void set_x_scale(float x_scale);
        [[nodiscard]] float& get_x_scale();
        void set_y_scale(float y_scale);
        [[nodiscard]] float& get_y_scale();

        [[nodiscard]] bool is_harmonic_enabled(size_t i) const;
        [[nodiscard]] bool is_only_harmonic_enabled(size_t i) const;
        void toggle_harmonic(size_t i);
        void solo_harmonic(size_t i);
        void reset_harmonic_mask();
        [[nodiscard]] const std::array<bool, max_harmonic_count>& get_harmonic_mask() const;

    private:
        int fps_{};
        raylib::Color background_color_{};
        boost::circular_buffer<float> wave_;
        boost::circular_buffer<raylib::Vector2> path_;
        bool is_paused_{};
        size_t selected_wave_index_{};
        unsigned int number_of_harmonic_{};
        float frequency_{};
        float radius_{};
        float phase_{};
        float x_scale_{};
        float y_scale_{};
        std::array<bool, max_harmonic_count> harmonic_enabled_{};
    };

} // namespace UI
