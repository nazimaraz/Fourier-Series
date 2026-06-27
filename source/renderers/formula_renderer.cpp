//
// Created by nazim on 6/27/26.
//

#include <memory>
#include <optional>
#include <string>
#include <texrender/headless_surface.hpp>
#include <texrender/render_handle.hpp>
#include <raylib.h>
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
    auto surface = std::make_unique<TeXRender::HeadlessSurface>();
    surface_ = surface.get();
    document_.emplace(std::string{TEXRENDER_RES_DIR}, std::move(surface));
}

FormulaRenderer::~FormulaRenderer()
{
    free_texture(static_texture_, static_valid_);
    free_texture(dynamic_texture_, dynamic_valid_);
}

void FormulaRenderer::free_texture(const raylib::Texture2D& texture, bool& valid)
{
    if (valid)
        raylib::UnloadTexture(texture);

    valid = false;
}

void FormulaRenderer::build_texture(raylib::Texture2D& texture, bool& valid, const std::string& latex, const float latex_text_size,
    const std::uint32_t argb_color) const
{
    if (latex.empty() || surface_ == nullptr)
    {
        valid = false;
        return;
    }

    const auto handle = document_->render(latex, latex_text_size, argb_color);
    const auto w = handle.width();
    const auto h = handle.height();
    if (w <= 0 || h <= 0)
    {
        valid = false;
        return;
    }

    // Rasterize the formula into the headless RGBA buffer with a transparent background.
    surface_->resize(w, h);
    surface_->clear(0x00000000);
    handle.draw(0, 0);
    if (!valid || texture.width != w || texture.height != h)
    {
        free_texture(texture, valid);
        const auto image = raylib::Image{
            .data = const_cast<void*>(static_cast<const void*>(surface_->pixels().data())),
            .width = w,
            .height = h,
            .mipmaps = 1,
            .format = raylib::PixelFormat::PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
        };
        texture = raylib::LoadTextureFromImage(image);
        valid = texture.id != 0;
    }
    else
    {
        raylib::UpdateTexture(texture, surface_->pixels().data());
    }
}

void FormulaRenderer::rebuild_static(const size_t wave_index) const
{
    const auto latex = Waves::formula_tex(wave_index);
    build_texture(static_texture_, static_valid_, std::string{latex}, text_size, color);
}

void FormulaRenderer::rebuild_dynamic(const DynamicSignature& signature) const
{
    const auto latex =
        Waves::dynamic_formula_tex(signature.wave_index, signature.harmonic_count, signature.radius, signature.frequency);
    build_texture(dynamic_texture_, dynamic_valid_, latex, dynamic_text_size, dynamic_color);
}

void FormulaRenderer::draw() const
{
    const auto wave_index = settings_.get_selected_wave_index();
    if (wave_index != last_wave_index_)
    {
        rebuild_static(wave_index);
        // Wave type change invalidates the dynamic expansion too.
        last_dynamic_.wave_index = static_cast<size_t>(-1);
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

    // Place the formulas at the top-left, just to the right of the ImGui "Settings" window.
    const auto left_x = panel_right_ + panel_gap;

    if (static_valid_)
    {
        const auto position = raylib::Vector2{left_x, margin_top};
        raylib::DrawTextureV(static_texture_, position, raylib::WHITE);
    }

    if (dynamic_valid_)
    {
        const auto static_h = static_valid_ ? static_cast<float>(static_texture_.height) : 0.f;
        const auto y = margin_top + static_h + vertical_gap;
        const auto position = raylib::Vector2{left_x, y};
        raylib::DrawTextureV(dynamic_texture_, position, raylib::WHITE);
    }
}

void FormulaRenderer::set_panel_right(const float x) const
{
    panel_right_ = x;
}
