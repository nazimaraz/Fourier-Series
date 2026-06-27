//
// Created by nazim on 6/27/26.
//

#pragma once

#include <cstddef>
#include <memory>
#include <optional>
#include <texrender/latex_document.hpp>
#include "vector2.hpp"

namespace TeXRender
{
    class HeadlessSurface;
}

namespace UI
{
    class Settings;
}

namespace Renderers
{
    // Renders the selected wave's Fourier-series formula as LaTeX via TeXRender's
    // headless surface, then uploads the resulting RGBA buffer to a raylib texture
    // and blits it onto the window. The texture is rebuilt only when the wave type
    // changes, so steady-state frames cost a single DrawTextureV.
    class FormulaRenderer
    {
    public:
        explicit FormulaRenderer(UI::Settings& settings);
        FormulaRenderer(const FormulaRenderer&) = delete;
        FormulaRenderer(FormulaRenderer&&) = delete;
        FormulaRenderer& operator=(const FormulaRenderer&) = delete;
        FormulaRenderer& operator=(FormulaRenderer&&) = delete;
        ~FormulaRenderer();

        void draw() const;

    private:
        void rebuild_texture(size_t wave_index) const;

        UI::Settings& settings_;
        TeXRender::HeadlessSurface* surface_{};
        std::optional<TeXRender::LatexDocument> document_;
        mutable raylib::Texture2D texture_{};
        mutable bool texture_valid_{};
        mutable size_t last_wave_index_{static_cast<size_t>(-1)};
    };
} // namespace Renderers
