//
// Created by nazim on 6/27/26.
//

#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <texrender/latex_document.hpp>
#include <texrender/render_handle.hpp>

namespace TeXRender
{
    class RaylibSurface;
}

namespace UI
{
    class Settings;
}

namespace Renderers
{
    class FormulaRenderer
    {
    public:
        explicit FormulaRenderer(UI::Settings& settings);
        void draw(float panel_right);

    private:
        struct DynamicSignature
        {
            std::size_t wave_index{static_cast<std::size_t>(-1)};
            unsigned int harmonic_count{};
            float radius{};
            float frequency{};

            [[nodiscard]] friend bool operator==(const DynamicSignature& lhs, const DynamicSignature& rhs)
            {
                return lhs.wave_index == rhs.wave_index && lhs.harmonic_count == rhs.harmonic_count && lhs.radius == rhs.radius &&
                       lhs.frequency == rhs.frequency;
            }
        };

        void rebuild_static(std::size_t wave_index);
        void rebuild_dynamic(const DynamicSignature& signature);
        void rebuild_handle(std::optional<TeXRender::RenderHandle>& handle, const std::string& latex, float latex_text_size,
            std::uint32_t argb_color) const;

        UI::Settings& settings_;
        std::optional<TeXRender::LatexDocument> document_;
        std::optional<TeXRender::RenderHandle> static_formula_;
        std::size_t last_wave_index_{static_cast<std::size_t>(-1)};
        std::optional<TeXRender::RenderHandle> dynamic_formula_;
        DynamicSignature last_dynamic_{};
    };
} // namespace Renderers
