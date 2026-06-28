//
// Created by Nazım Can on 25.10.2025.
//

#include <cstddef>
#include <imgui.h>
#include <rlImGui.h>
#include "gui.hpp"
#include "config.hpp"
#include "gui/drawing_input.hpp"
#include "renderers/chart_renderer.hpp"
#include "renderers/formula_renderer.hpp"
#include "renderers/spectrum_renderer.hpp"
#include "renderers/wave_renderer.hpp"
#include "waves/wave_variant.hpp"

GUI::GUI() = default;

GUI::~GUI() = default;

auto GUI::initialize() -> void
{
    InitWindow(Config::Window::screen_width, Config::Window::screen_height, "Fourier Series");
    SetTargetFPS(Config::Window::target_fps);
    rlImGuiSetup(true);
    settings_.set_background_color(Config::Defaults::background_color);
    settings_.set_wave_capacity(Config::Defaults::wave_capacity);
    settings_.set_path_capacity(Config::Defaults::path_capacity);
    settings_.set_number_of_harmonic(Config::Defaults::harmonic_count);
    settings_.set_frequency(Config::Defaults::frequency);
    settings_.set_radius(Config::Defaults::radius);
    settings_.set_x_scale(Config::Defaults::x_scale);
    settings_.set_y_scale(Config::Defaults::y_scale);
    settings_.set_selected_wave_index(Waves::index_of<Waves::Square>);
    chart_renderer_ = std::make_unique<Renderers::ChartRenderer>(settings_);
    wave_renderer_ = std::make_unique<Renderers::WaveRenderer>(settings_);
    spectrum_renderer_ = std::make_unique<Renderers::SpectrumRenderer>(settings_);
    formula_renderer_ = std::make_unique<Renderers::FormulaRenderer>(settings_);
    drawing_input_ = std::make_unique<UI::DrawingInput>(settings_);
}

auto GUI::update() -> void
{
    if (WindowShouldClose())
    {
        stop();
        return;
    }

    BeginDrawing();
    update_impl();
    EndDrawing();
}

auto GUI::stop() -> void
{
    CloseWindow();
    is_stopped_ = true;
}

auto GUI::is_stopped() const -> bool
{
    return is_stopped_;
}

auto GUI::update_impl() -> void
{
    ClearBackground(settings_.get_background_color());
    rlImGuiBegin();
    ImGui::SetNextWindowSizeConstraints({200.f, static_cast<float>(GetScreenHeight())},
        {static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())});
    ImGui::SetNextWindowPos({0.f, 0.f});
    ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
    update_settings();
    const auto panel_right = ImGui::GetWindowWidth();
    ImGui::End();
    rlImGuiEnd();
    drawing_input_->handle();
    chart_renderer_->draw();
    wave_renderer_->draw();
    spectrum_renderer_->draw();
    formula_renderer_->draw(panel_right);
    if (!settings_.get_is_paused())
        settings_.set_phase(settings_.get_phase() + settings_.frequency() * GetFrameTime());
}

auto GUI::update_settings() -> void
{
    settings_.set_fps(GetFPS());
    ImGui::Text("FPS: %d", settings_.get_fps());
    if (ImGui::Button("Clear"))
    {
        settings_.wave().clear();
        settings_.path().clear();
    }

    ImGui::SameLine();
    if (settings_.get_is_paused())
    {
        if (ImGui::Button("Continue"))
            settings_.set_is_paused(false);
    }
    else
    {
        if (ImGui::Button("Pause"))
            settings_.set_is_paused(true);
    }

    {
        auto selected_wave_index = static_cast<int>(settings_.get_selected_wave_index());
        ImGui::Text("Wave Type:");
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::Combo("##WaveType", &selected_wave_index, Waves::wave_names_cstr.data(), Waves::wave_names_cstr.size()))
            settings_.set_selected_wave_index(static_cast<std::size_t>(selected_wave_index));

        ImGui::PopItemWidth();
    }
    static constexpr auto number_of_harmonic_min = 1u;
    static constexpr auto number_of_harmonic_max = 100u;
    ImGui::Text("Number of Harmonic:");
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::SliderScalar("##NumberOfHarmonic", ImGuiDataType_U32, &settings_.mutable_number_of_harmonic(),
        &number_of_harmonic_min, &number_of_harmonic_max);
    ImGui::PopItemWidth();

    ImGui::Text("Frequency:");
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::SliderFloat("##Frequency", &settings_.mutable_frequency(), 0.f, 2.f);
    ImGui::PopItemWidth();

    ImGui::Text("Radius:");
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::SliderFloat("##Radius", &settings_.mutable_radius(), 0.1f, 200.f);
    ImGui::PopItemWidth();

    ImGui::Text("Signal X Scale:");
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::SliderFloat("##XScale", &settings_.mutable_x_scale(), 0.1f, 5.f);
    ImGui::PopItemWidth();

    ImGui::Text("Signal Y Scale:");
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::SliderFloat("##YScale", &settings_.mutable_y_scale(), 0.1f, 5.f);
    ImGui::PopItemWidth();
}
