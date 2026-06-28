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
        auto set_fps(int fps) -> void;
        [[nodiscard]] auto get_fps() const -> int;
        auto set_background_color(Color color) -> void;
        [[nodiscard]] auto get_background_color() const -> Color;
        auto set_wave_capacity(boost::circular_buffer<float>::capacity_type capacity) -> void;
        [[nodiscard]] auto wave() -> boost::circular_buffer<float>&;
        auto set_path_capacity(boost::circular_buffer<Vector2>::capacity_type capacity) -> void;
        [[nodiscard]] auto path() -> boost::circular_buffer<Vector2>&;
        auto set_is_paused(bool is_paused) -> void;
        [[nodiscard]] auto get_is_paused() const -> bool;
        auto set_selected_wave_index(std::size_t index) -> void;
        [[nodiscard]] auto get_selected_wave_index() const -> std::size_t;
        [[nodiscard]] auto get_selected_wave() const -> const Waves::WaveVariant&;
        [[nodiscard]] auto get_selected_wave() -> Waves::WaveVariant&;
        auto start_drawing() -> void;
        auto stop_drawing() -> void;
        auto add_drawing_point(Vector2 p) -> void;
        auto set_drawing_points(std::vector<Vector2> points) -> void;
        auto set_selected_wave(Waves::WaveVariant wave) -> void;
        [[nodiscard]] auto is_drawing() const -> bool;
        [[nodiscard]] auto get_drawing_points() const -> const std::vector<Vector2>&;
        auto set_number_of_harmonic(unsigned int number) -> void;
        [[nodiscard]] auto number_of_harmonic() const -> unsigned int;
        [[nodiscard]] auto mutable_number_of_harmonic() -> unsigned int&;
        auto set_frequency(float frequency) -> void;
        [[nodiscard]] auto frequency() const -> float;
        [[nodiscard]] auto mutable_frequency() -> float&;
        auto set_radius(float radius) -> void;
        [[nodiscard]] auto radius() const -> float;
        [[nodiscard]] auto mutable_radius() -> float&;
        auto set_phase(float phase) -> void;
        [[nodiscard]] auto get_phase() const -> float;
        auto set_x_scale(float x_scale) -> void;
        [[nodiscard]] auto x_scale() const -> float;
        [[nodiscard]] auto mutable_x_scale() -> float&;
        auto set_y_scale(float y_scale) -> void;
        [[nodiscard]] auto y_scale() const -> float;
        [[nodiscard]] auto mutable_y_scale() -> float&;
        [[nodiscard]] auto is_harmonic_enabled(std::size_t i) const -> bool;
        [[nodiscard]] auto is_only_harmonic_enabled(std::size_t i) const -> bool;
        auto toggle_harmonic(std::size_t i) -> void;
        auto solo_harmonic(std::size_t i) -> void;
        auto reset_harmonic_mask() -> void;
        [[nodiscard]] auto get_harmonic_mask() const -> const std::array<bool, max_harmonic_count>&;

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
