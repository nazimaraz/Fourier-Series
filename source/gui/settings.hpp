//
// Created by Nazım Can on 25.10.2025.
//

#pragma once

#include <array>
#include <cstddef>
#include <vector>
#include <boost/circular_buffer.hpp>
#include <raylib.h>
#include "waves/wave_variant.hpp"

namespace UI
{
    class Settings
    {
    public:
        static constexpr std::size_t max_harmonic_count = 100;

        Settings();
        void set_fps(int fps);
        [[nodiscard]] int get_fps() const;
        void set_background_color(Color color);
        [[nodiscard]] Color get_background_color() const;
        void set_wave_capacity(boost::circular_buffer<float>::capacity_type capacity);
        [[nodiscard]] boost::circular_buffer<float>& get_wave();
        void set_path_capacity(boost::circular_buffer<Vector2>::capacity_type capacity);
        [[nodiscard]] boost::circular_buffer<Vector2>& get_path();
        void set_is_paused(bool is_paused);
        [[nodiscard]] bool get_is_paused() const;
        void set_selected_wave_index(std::size_t index);
        [[nodiscard]] std::size_t get_selected_wave_index() const;
        [[nodiscard]] const Waves::WaveVariant& get_selected_wave() const;
        [[nodiscard]] Waves::WaveVariant& get_selected_wave();
        void start_drawing();
        void stop_drawing();
        void add_drawing_point(Vector2 p);
        void set_drawing_points(std::vector<Vector2> points);
        void set_selected_wave(Waves::WaveVariant wave);
        [[nodiscard]] bool is_drawing() const;
        [[nodiscard]] const std::vector<Vector2>& get_drawing_points() const;
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
        [[nodiscard]] bool is_harmonic_enabled(std::size_t i) const;
        [[nodiscard]] bool is_only_harmonic_enabled(std::size_t i) const;
        void toggle_harmonic(std::size_t i);
        void solo_harmonic(std::size_t i);
        void reset_harmonic_mask();
        [[nodiscard]] const std::array<bool, max_harmonic_count>& get_harmonic_mask() const;

    private:
        int fps_{};
        Color background_color_{};
        boost::circular_buffer<float> wave_;
        boost::circular_buffer<Vector2> path_;
        bool is_paused_{};
        Waves::WaveVariant selected_wave_;
        std::vector<Vector2> drawing_points_;
        bool is_drawing_{};
        unsigned int number_of_harmonic_{};
        float frequency_{};
        float radius_{};
        float phase_{};
        float x_scale_{};
        float y_scale_{};
        std::array<bool, max_harmonic_count> harmonic_enabled_{};
    };

} // namespace UI
