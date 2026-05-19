//
// Created by Nazım Can on 20.05.2026.
//

#pragma once

#include <memory>

namespace UI
{
    class Settings;
}

namespace Renderers
{
    class WaveRenderer
    {
    public:
        explicit WaveRenderer(std::shared_ptr<UI::Settings> settings);
        void draw() const;

    private:
        std::shared_ptr<UI::Settings> settings_;
    };
} // namespace Renderers
