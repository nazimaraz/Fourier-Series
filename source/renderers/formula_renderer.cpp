//
// Created by nazim on 6/27/26.
//

#include <memory>
#include <optional>
#include <texrender/draw_surface.hpp>
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
    if (texture_valid_)
        raylib::UnloadTexture(texture_);
}

void FormulaRenderer::rebuild_texture(const size_t wave_index) const
{
    const auto latex = Waves::formula_tex(wave_index);
    if (latex.empty() || surface_ == nullptr)
    {
        texture_valid_ = false;
        return;
    }

    const auto handle = document_->render(std::string{latex}, text_size, color);
    const auto w = handle.width();
    const auto h = handle.height();
    if (w <= 0 || h <= 0)
    {
        texture_valid_ = false;
        return;
    }

    // Rasterize the formula into the headless RGBA buffer with a transparent background.
    surface_->resize(w, h);
    surface_->clear(0x00000000);
    handle.draw(0, 0);

    if (!texture_valid_ || texture_.width != w || texture_.height != h)
    {
        if (texture_valid_)
            raylib::UnloadTexture(texture_);

        auto image = raylib::Image{};
        image.data = const_cast<void*>(static_cast<const void*>(surface_->pixels().data()));
        image.width = w;
        image.height = h;
        image.mipmaps = 1;
        image.format = raylib::PixelFormat::PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
        texture_ = raylib::LoadTextureFromImage(image);
        texture_valid_ = texture_.id != 0;
    }
    else
    {
        raylib::UpdateTexture(texture_, surface_->pixels().data());
    }
}

void FormulaRenderer::draw() const
{
    const auto wave_index = settings_.get_selected_wave_index();
    if (wave_index != last_wave_index_)
    {
        rebuild_texture(wave_index);
        last_wave_index_ = wave_index;
    }

    if (!texture_valid_)
        return;

    const auto screen_w = static_cast<float>(raylib::GetScreenWidth());
    const auto position = raylib::Vector2{screen_w - static_cast<float>(texture_.width) - margin_right, margin_top};
    raylib::DrawTextureV(texture_, position, raylib::WHITE);
}
