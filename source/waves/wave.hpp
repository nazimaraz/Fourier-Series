//
// Created by Nazım Can on 25.10.2025.
//

#pragma once

#include <string>

#include "function.h"
#include "gui/settings.hpp"

namespace UI
{
    class Settings;
}

namespace Waves
{
    class Wave {
    public:
        Wave(std::string name, Type type);
        virtual ~Wave();

        void update() const;
        [[nodiscard]] virtual std::pair<float, float> get_formula(float i) const = 0;
        void set_name(const std::string& name);
        [[nodiscard]] const std::string& get_name() const;
        void set_type(Type type);
        [[nodiscard]] Type get_type() const;
        void set_settings(const std::shared_ptr<UI::Settings>& settings);
        [[nodiscard]] const std::shared_ptr<UI::Settings>& get_settings() const;

    protected:
        std::shared_ptr<UI::Settings> settings_;

    private:
        std::string name_;
        Type type_{};
    };
}
