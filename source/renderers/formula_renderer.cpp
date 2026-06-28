//
// Created by nazim on 6/27/26.
//

#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <texrender/raylib_surface.hpp>
#include <texrender/render_handle.hpp>
#include "formula_renderer.hpp"
#include "config.hpp"
#include "gui/settings.hpp"
#include "waves/formula_tex.hpp"

#ifndef TEXRENDER_RES_DIR
#   define TEXRENDER_RES_DIR "res"
#endif

using namespace Renderers;
using namespace Config::Formula;

FormulaRenderer::FormulaRenderer(UI::Settings& settings)
    : settings_{settings}
{
    auto surface = std::make_unique<TeXRender::RaylibSurface>();
    document_.emplace(std::string{TEXRENDER_RES_DIR}, std::move(surface));
}

void FormulaRenderer::rebuild_handle(std::optional<TeXRender::RenderHandle>& handle, const std::string& latex,
    const float latex_text_size, const std::uint32_t argb_color) const
{
    if (latex.empty())
    {
        handle.reset();
        return;
    }

    auto rendered = document_->render(latex, latex_text_size, argb_color);
    if (rendered.width() <= 0 || rendered.height() <= 0)
    {
        handle.reset();
        return;
    }

    handle.emplace(std::move(rendered));
}

void FormulaRenderer::rebuild_static(const std::size_t wave_index)
{
    const auto latex = Waves::formula_tex(wave_index);
    rebuild_handle(static_formula_, std::string{latex}, text_size, color);
}

void FormulaRenderer::rebuild_dynamic(const DynamicSignature& signature)
{
    const auto latex =
        Waves::dynamic_formula_tex(signature.wave_index, signature.harmonic_count, signature.radius, signature.frequency);
    rebuild_handle(dynamic_formula_, latex, dynamic_text_size, dynamic_color);
}

void FormulaRenderer::draw(const float panel_right)
{
    const auto wave_index = settings_.get_selected_wave_index();
    if (wave_index != last_wave_index_)
    {
        rebuild_static(wave_index);
        last_dynamic_.wave_index = static_cast<std::size_t>(-1);
        last_wave_index_ = wave_index;
    }

    const auto signature = DynamicSignature{
        .wave_index = wave_index,
        .harmonic_count = settings_.get_number_of_harmonic(),
        .radius = settings_.get_radius(),
        .frequency = settings_.get_frequency(),
    };
    if (signature != last_dynamic_)
    {
        rebuild_dynamic(signature);
        last_dynamic_ = signature;
    }

    const auto left_x = panel_right + panel_gap;
    if (static_formula_)
        static_formula_->draw(static_cast<int>(left_x), static_cast<int>(margin_top));

    if (dynamic_formula_)
    {
        const auto static_h = static_formula_ ? static_cast<float>(static_formula_->height()) : 0.f;
        const auto y = margin_top + static_h + vertical_gap;
        dynamic_formula_->draw(static_cast<int>(left_x), static_cast<int>(y));
    }
}
