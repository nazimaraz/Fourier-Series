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
        auto draw(float panel_right) -> void;

    private:
        struct DynamicSignature
        {
            std::size_t wave_index{static_cast<std::size_t>(-1)};
            unsigned int harmonic_count{};
            float radius{};
            float frequency{};

            [[nodiscard]] friend auto operator==(const DynamicSignature& lhs, const DynamicSignature& rhs) -> bool
            {
                return lhs.wave_index == rhs.wave_index && lhs.harmonic_count == rhs.harmonic_count && lhs.radius == rhs.radius &&
                       lhs.frequency == rhs.frequency;
            }
        };

        auto rebuild_static(std::size_t wave_index) -> void;
        auto rebuild_dynamic(const DynamicSignature& signature) -> void;
        auto rebuild_handle(std::optional<TeXRender::RenderHandle>& handle, const std::string& latex, float latex_text_size,
            std::uint32_t argb_color) const -> void;

        UI::Settings& settings_;
        std::optional<TeXRender::LatexDocument> document_;
        std::optional<TeXRender::RenderHandle> static_formula_;
        std::size_t last_wave_index_{static_cast<std::size_t>(-1)};
        std::optional<TeXRender::RenderHandle> dynamic_formula_;
        DynamicSignature last_dynamic_{};
    };
} // namespace Renderers
