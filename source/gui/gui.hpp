//
// Created by Nazım Can on 25.10.2025.
//

#pragma once

#include <memory>
#include "gui/settings.hpp"

namespace UI
{
    class DrawingInput;
} // namespace UI

namespace Renderers
{
    class ChartRenderer;
    class WaveRenderer;
    class FormulaRenderer;
} // namespace Renderers

class GUI final
{
public:
    GUI();
    GUI(const GUI&) = delete;
    GUI(GUI&&) = delete;
    auto operator=(const GUI&) -> GUI& = delete;
    auto operator=(GUI&&) -> GUI& = delete;
    ~GUI();

    auto initialize() -> void;
    auto update() -> void;
    auto stop() -> void;
    [[nodiscard]] auto is_stopped() const -> bool;

private:
    auto update_impl() -> void;
    auto update_settings() -> void;

    bool is_stopped_{};
    UI::Settings settings_;
    std::unique_ptr<Renderers::ChartRenderer> chart_renderer_;
    std::unique_ptr<Renderers::WaveRenderer> wave_renderer_;
    std::unique_ptr<Renderers::FormulaRenderer> formula_renderer_;
    std::unique_ptr<UI::DrawingInput> drawing_input_;
};
