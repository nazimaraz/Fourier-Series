//
// Created by nazim on 6/27/26.
//

#include <cstdint>
#include <cstring>
#include <memory>
#include <optional>
#include <string>
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
    free_texture(static_texture_, static_valid_);
    free_texture(dynamic_texture_, dynamic_valid_);
}

void FormulaRenderer::free_texture(raylib::Texture2D& texture, bool& valid) const
{
    if (valid)
        raylib::UnloadTexture(texture);
    valid = false;
}

void FormulaRenderer::build_texture(raylib::Texture2D& texture, bool& valid, const std::string& latex, const float text_size,
    const std::uint32_t color) const
{
    if (latex.empty() || surface_ == nullptr)
    {
        valid = false;
        return;
    }

    const auto handle = document_->render(latex, text_size, color);
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
        auto image = raylib::Image{};
        image.data = const_cast<void*>(static_cast<const void*>(surface_->pixels().data()));
        image.width = w;
        image.height = h;
        image.mipmaps = 1;
        image.format = raylib::PixelFormat::PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
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
    const auto& mask = settings_.get_harmonic_mask();
    const auto latex = Waves::dynamic_formula_tex(settings_.get_selected_wave(), signature.harmonic_count, signature.radius,
        signature.frequency, mask.data());
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

    const auto& mask = settings_.get_harmonic_mask();
    // FNV-1a over the prefix of the mask that can actually be referenced by the harmonic count.
    auto mask_hash = std::uint64_t{0xcbf29ce484222325ull};
    for (auto i = size_t{}; i < settings_.get_number_of_harmonic(); ++i)
    {
        mask_hash ^= mask[i] ? std::uint64_t{1} : std::uint64_t{0};
        mask_hash *= 0x100000001b3ull;
    }

    const auto signature = DynamicSignature{
        .wave_index = wave_index,
        .harmonic_count = settings_.get_number_of_harmonic(),
        .radius = settings_.get_radius(),
        .frequency = settings_.get_frequency(),
        .mask_hash = mask_hash,
    };
    if (std::memcmp(&signature, &last_dynamic_, sizeof(DynamicSignature)) != 0)
    {
        rebuild_dynamic(signature);
        last_dynamic_ = signature;
    }

    const auto screen_w = static_cast<float>(raylib::GetScreenWidth());
    const auto right_x = screen_w - margin_right;

    if (static_valid_)
    {
        const auto position = raylib::Vector2{right_x - static_cast<float>(static_texture_.width), margin_top};
        raylib::DrawTextureV(static_texture_, position, raylib::WHITE);
    }

    if (dynamic_valid_)
    {
        const auto static_h = static_valid_ ? static_cast<float>(static_texture_.height) : 0.f;
        const auto y = margin_top + static_h + vertical_gap;
        const auto position = raylib::Vector2{right_x - static_cast<float>(dynamic_texture_.width), y};
        raylib::DrawTextureV(dynamic_texture_, position, raylib::WHITE);
    }
}
