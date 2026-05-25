//
// Created by nazim on 5/23/26.
//

#pragma once

#include "vector2.hpp"

namespace UI
{
    class Settings;
}

namespace Renderers
{
    class ChartRenderer
    {
    public:
        explicit ChartRenderer(UI::Settings& settings);
        ChartRenderer(const ChartRenderer&) = delete;
        ChartRenderer(ChartRenderer&&) = delete;
        ChartRenderer& operator=(const ChartRenderer&) = delete;
        ChartRenderer& operator=(ChartRenderer&&) = delete;
        ~ChartRenderer();

        void draw() const;

        static constexpr float origin_x = 800.f;
        static constexpr float origin_y = 450.f;

    private:
        UI::Settings& settings_;
    };
} // namespace Renderers
