//
// Created by nazim on 5/23/26.
//

#pragma once

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
        auto draw() const -> void;

    private:
        UI::Settings& settings_;
    };
} // namespace Renderers
