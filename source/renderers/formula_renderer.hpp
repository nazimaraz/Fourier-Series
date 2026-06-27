//
// Created by nazim on 6/27/26.
//

#pragma once

#include <cstddef>
#include <cstdint>
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
    // Renders the selected wave's Fourier-series formula onto the window as LaTeX via TeXRender's
    // headless surface, uploading each rasterized RGBA buffer to a raylib texture. Two formulas are
    // shown: the static closed-form series (top) and a dynamic one (below) that expands the actual
    // terms the renderer is drawing, rebuilt live as the harmonic count, frequency, radius, or
    // harmonic mask change. Textures are rebuilt only on change, so steady-state frames cost two
    // DrawTextureV calls.
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
        struct DynamicSignature
        {
            size_t wave_index{static_cast<size_t>(-1)};
            unsigned int harmonic_count{};
            float radius{};
            float frequency{};
            std::uint64_t mask_hash{};
        };

        void rebuild_static(size_t wave_index) const;
        void rebuild_dynamic(const DynamicSignature& signature) const;
        void build_texture(raylib::Texture2D& texture, bool& valid, const std::string& latex, float text_size,
            std::uint32_t color) const;
        void free_texture(raylib::Texture2D& texture, bool& valid) const;

        UI::Settings& settings_;
        TeXRender::HeadlessSurface* surface_{};
        std::optional<TeXRender::LatexDocument> document_;
        mutable raylib::Texture2D static_texture_{};
        mutable bool static_valid_{};
        mutable size_t last_wave_index_{static_cast<size_t>(-1)};
        mutable raylib::Texture2D dynamic_texture_{};
        mutable bool dynamic_valid_{};
        mutable DynamicSignature last_dynamic_{};
    };
} // namespace Renderers
