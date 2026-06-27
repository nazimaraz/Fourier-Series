//
// Created by nazim on 5/26/26.
//

#pragma once

namespace UI
{
    class Settings;
}

namespace Renderers
{
    class SpectrumRenderer
    {
    public:
        explicit SpectrumRenderer(UI::Settings& settings);
        void draw() const;

    private:
        UI::Settings& settings_;
    };
} // namespace Renderers
