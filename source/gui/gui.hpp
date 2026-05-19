//
// Created by Nazım Can on 25.10.2025.
//

#pragma once

#include <memory>

namespace UI
{
    class Settings;
}

class GUI final
{
public:
    void initialize();
    void update();
    void stop();
    [[nodiscard]] bool is_stopped() const;

private:
    void update_impl() const;
    void update_settings() const;

    bool is_stopped_{};
    std::shared_ptr<UI::Settings> settings_;
};
