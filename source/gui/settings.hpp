//
// Created by Nazım Can on 25.10.2025.
//

#pragma once

#include <boost/bimap.hpp>
#include <boost/circular_buffer.hpp>
#include <raylib/color.h>
#include <unordered_map>

namespace Waves
{
    class Wave;
    enum class Type : std::uint8_t;
}

namespace UI
{
    class Settings : public std::enable_shared_from_this<Settings> {
    public:
        void set_fps(int fps);
        [[nodiscard]] int get_fps() const;
        void set_background_color(raylib::Color color);
        [[nodiscard]] raylib::Color get_background_color() const;
        void set_wave_capacity(boost::circular_buffer<float>::capacity_type capacity);
        boost::circular_buffer<float>& get_wave();
        void set_is_paused(bool is_paused);
        [[nodiscard]] bool get_is_paused() const;
        template <typename ...Ts>
        void add_waves();
        void add_wave(const std::shared_ptr<Waves::Wave>& wave);
        const std::shared_ptr<Waves::Wave>& get_wave(const std::string& name);
        const std::shared_ptr<Waves::Wave>& get_wave(Waves::Type type);
        [[nodiscard]] const std::vector<const char*>& get_wave_names() const;
        [[nodiscard]] const std::string& get_wave_name(Waves::Type type) const;
        [[nodiscard]] Waves::Type get_wave_type(const std::string& name) const;
        void set_selected_wave(const std::shared_ptr<Waves::Wave>& wave);
        void set_selected_wave(Waves::Type type);
        void set_selected_wave(const std::string& name);
        [[nodiscard]] Waves::Type get_selected_wave_type() const;
        [[nodiscard]] const std::shared_ptr<Waves::Wave>& get_selected_wave() const;
        void set_number_of_harmonic(unsigned int number);
        [[nodiscard]] unsigned int& get_number_of_harmonic();
        void set_frequency(float frequency);
        [[nodiscard]] float& get_frequency();
        void set_radius(float radius);
        [[nodiscard]] float& get_radius();
        void set_time(float time);
        float get_time() const;

    private:
        int fps_{};
        raylib::Color background_color_{};
        boost::circular_buffer<float> wave_{};
        bool is_paused_{};
        std::unordered_map<Waves::Type, std::shared_ptr<Waves::Wave>> waves_;
        std::unordered_map<std::string, std::shared_ptr<Waves::Wave>> wave_name_waves_;
        boost::bimap<Waves::Type, std::string> wave_names_types_;
        std::vector<const char*> wave_names_;
        Waves::Type selected_wave_type_{};
        unsigned int number_of_harmonic_{};
        float frequency_{};
        float radius_{};
        float time_{};
    };

}

#include "settings.tpp"
