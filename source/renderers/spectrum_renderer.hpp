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
        SpectrumRenderer(const SpectrumRenderer&) = delete;
        SpectrumRenderer(SpectrumRenderer&&) = delete;
        SpectrumRenderer& operator=(const SpectrumRenderer&) = delete;
        SpectrumRenderer& operator=(SpectrumRenderer&&) = delete;
        ~SpectrumRenderer();

        void draw() const;

    private:
        UI::Settings& settings_;
    };
} // namespace Renderers
