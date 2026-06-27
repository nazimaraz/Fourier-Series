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
    // shown: the static closed-form series (top) and a dynamic one (below) that is the same original
    // formula with the live values plugged in — the infinite sum bound becomes the harmonic count,
    // the time variable becomes omega*t, and the series is scaled by the radius. The dynamic texture
    // rebuilds live as the harmonic count, frequency, or radius change. Textures are rebuilt only on
    // change, so steady-state frames cost two DrawTextureV calls.
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
        void set_panel_right(float x) const;

    private:
        struct DynamicSignature
        {
            size_t wave_index{static_cast<size_t>(-1)};
            unsigned int harmonic_count{};
            float radius{};
            float frequency{};

            [[nodiscard]] friend bool operator==(const DynamicSignature& lhs, const DynamicSignature& rhs)
            {
                return lhs.wave_index == rhs.wave_index && lhs.harmonic_count == rhs.harmonic_count && lhs.radius == rhs.radius &&
                       lhs.frequency == rhs.frequency;
            }
        };

        void rebuild_static(size_t wave_index) const;
        void rebuild_dynamic(const DynamicSignature& signature) const;
        void build_texture(raylib::Texture2D& texture, bool& valid, const std::string& latex, float text_size,
            std::uint32_t color) const;
        static void free_texture(const raylib::Texture2D& texture, bool& valid);

        UI::Settings& settings_;
        TeXRender::HeadlessSurface* surface_{};
        std::optional<TeXRender::LatexDocument> document_;
        mutable float panel_right_{};
        mutable raylib::Texture2D static_texture_{};
        mutable bool static_valid_{};
        mutable size_t last_wave_index_{static_cast<size_t>(-1)};
        mutable raylib::Texture2D dynamic_texture_{};
        mutable bool dynamic_valid_{};
        mutable DynamicSignature last_dynamic_{};
    };
} // namespace Renderers
