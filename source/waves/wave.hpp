//
// Created by Nazım Can on 25.10.2025.
//

#pragma once

#include <memory>
#include <string>
#include "compute_result.hpp"
#include "harmonic_term.hpp"

namespace UI
{
    class Settings;
}

namespace Waves
{
    enum class Type : std::uint8_t;

    class Wave
    {
    public:
        Wave(std::string name, Type type);
        Wave(const Wave&) = delete;
        Wave(Wave&&) = delete;
        Wave& operator=(const Wave&) = delete;
        Wave& operator=(Wave&&) = delete;
        virtual ~Wave();

        [[nodiscard]] ComputeResult compute() const;
        [[nodiscard]] virtual HarmonicTerm get_formula(float i) const = 0;
        [[nodiscard]] virtual float get_dc() const;
        void set_name(std::string name);
        [[nodiscard]] const std::string& get_name() const;
        void set_type(Type type);
        [[nodiscard]] Type get_type() const;
        void set_settings(std::shared_ptr<UI::Settings> settings);
        [[nodiscard]] const std::shared_ptr<UI::Settings>& get_settings() const;

    protected:
        std::shared_ptr<UI::Settings> settings_;

    private:
        std::string name_;
        Type type_{};
    };
} // namespace Waves
