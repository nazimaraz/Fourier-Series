#pragma once

namespace UI
{
    template <typename ...Ts>
    void Settings::add_waves()
    {
        (add_wave(std::make_shared<Ts>()), ...);
    }
}
