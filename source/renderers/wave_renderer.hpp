//
// Created by Nazım Can on 20.05.2026.
//

#pragma once

#include <raylib.h>
#include <vector>

namespace UI
{
    class Settings;
}

namespace Renderers
{
    class WaveRenderer
    {
    public:
        explicit WaveRenderer(UI::Settings& settings);
        WaveRenderer(const WaveRenderer&) = delete;
        WaveRenderer(WaveRenderer&&) = delete;
        WaveRenderer& operator=(const WaveRenderer&) = delete;
        WaveRenderer& operator=(WaveRenderer&&) = delete;
        ~WaveRenderer();

        void draw() const;

    private:
        UI::Settings& settings_;
        mutable std::vector<Vector2> points_buffer_;
        mutable float last_phase_{};
        mutable bool initialized_{};
    };
} // namespace Renderers
