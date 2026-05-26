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
} // namespace Renderers

class GUI final
{
public:
    GUI();
    GUI(const GUI&) = delete;
    GUI(GUI&&) = delete;
    GUI& operator=(const GUI&) = delete;
    GUI& operator=(GUI&&) = delete;
    ~GUI();

    void initialize();
    void update();
    void stop();
    [[nodiscard]] bool is_stopped() const;

private:
    void update_impl() const;
    void update_settings() const;

    bool is_stopped_{};
    std::shared_ptr<UI::Settings> settings_;
    std::unique_ptr<Renderers::ChartRenderer> chart_renderer_;
    std::unique_ptr<Renderers::WaveRenderer> wave_renderer_;
    std::unique_ptr<Renderers::SpectrumRenderer> spectrum_renderer_;
    std::unique_ptr<UI::DrawingInput> drawing_input_;
};
