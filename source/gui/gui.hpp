//
// Created by Nazım Can on 25.10.2025.
//

#pragma once

#include <memory>

namespace UI
{
    class Settings;
    class DrawingInput;
} // namespace UI

namespace Renderers
{
    class ChartRenderer;
    class SpectrumRenderer;
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
    auto update_impl() const -> void;
    auto update_settings() const -> void;

    bool is_stopped_{};
    std::shared_ptr<UI::Settings> settings_;
    std::unique_ptr<Renderers::ChartRenderer> chart_renderer_;
    std::unique_ptr<Renderers::WaveRenderer> wave_renderer_;
    std::unique_ptr<Renderers::SpectrumRenderer> spectrum_renderer_;
    std::unique_ptr<Renderers::FormulaRenderer> formula_renderer_;
    std::unique_ptr<UI::DrawingInput> drawing_input_;
};
