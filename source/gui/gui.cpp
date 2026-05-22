//
// Created by Nazım Can on 25.10.2025.
//

#include <imgui.h>
#include <rlImGui.h>
#include "gui.hpp"
#include "gui/settings.hpp"
#include "renderers/wave_renderer.hpp"
#include "waves/wave_variant.hpp"

GUI::GUI() = default;

GUI::~GUI() = default;

void GUI::initialize()
{
    constexpr auto screen_width = 1600;
    constexpr auto screen_height = 900;
    raylib::InitWindow(screen_width, screen_height, "Fourier Series");
    raylib::SetTargetFPS(120);
    raylib::rlImGuiSetup(true);
    settings_ = std::make_shared<UI::Settings>();
    settings_->set_background_color(raylib::BLACK);
    settings_->set_wave_capacity(1000);
    settings_->set_path_capacity(2000);
    settings_->set_number_of_harmonic(10);
    settings_->set_frequency(0.1f);
    settings_->set_radius(100.f);
    settings_->set_x_scale(1.f);
    settings_->set_y_scale(1.f);
    settings_->set_selected_wave_index(Waves::index_of<Waves::Square>);
    wave_renderer_ = std::make_unique<Renderers::WaveRenderer>(*settings_);
}

void GUI::update()
{
    if (raylib::WindowShouldClose())
    {
        stop();
        return;
    }

    raylib::BeginDrawing();
    update_impl();
    raylib::EndDrawing();
}

void GUI::stop()
{
    raylib::CloseWindow();
    is_stopped_ = true;
}

bool GUI::is_stopped() const
{
    return is_stopped_;
}

void GUI::update_impl() const
{
    ClearBackground(settings_->get_background_color());
    raylib::rlImGuiBegin();
    ImGui::SetNextWindowSizeConstraints({200.f, static_cast<float>(raylib::GetScreenHeight())},
        {static_cast<float>(raylib::GetScreenWidth()), static_cast<float>(raylib::GetScreenHeight())});
    ImGui::SetNextWindowPos({0.f, 0.f});
    ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
    update_settings();
    ImGui::End();
    raylib::rlImGuiEnd();
    wave_renderer_->draw();
    if (!settings_->get_is_paused())
        settings_->set_phase(settings_->get_phase() + settings_->get_frequency() * raylib::GetFrameTime());
}

void GUI::update_settings() const
{
    settings_->set_fps(raylib::GetFPS());
    ImGui::Text("FPS: %d", settings_->get_fps());
    if (ImGui::Button("Clear"))
        settings_->get_wave().clear();

    ImGui::SameLine();
    if (settings_->get_is_paused())
    {
        if (ImGui::Button("Continue"))
            settings_->set_is_paused(false);
    }
    else
    {
        if (ImGui::Button("Pause"))
            settings_->set_is_paused(true);
    }

    {
        auto selected_wave_index = static_cast<int>(settings_->get_selected_wave_index());
        ImGui::Text("Wave Type:");
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::Combo("##WaveType", &selected_wave_index, Waves::wave_names_cstr.data(), Waves::wave_names_cstr.size());
        ImGui::PopItemWidth();
        settings_->set_selected_wave_index(static_cast<size_t>(selected_wave_index));
    }
    static constexpr auto number_of_harmonic_min = 1u;
    static constexpr auto number_of_harmonic_max = 100u;
    ImGui::Text("Number of Harmonic:");
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::SliderScalar("##NumberOfHarmonic", ImGuiDataType_U32, &settings_->get_number_of_harmonic(), &number_of_harmonic_min,
        &number_of_harmonic_max);
    ImGui::PopItemWidth();

    ImGui::Text("Frequency:");
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::SliderFloat("##Frequency", &settings_->get_frequency(), 0.f, 2.f);
    ImGui::PopItemWidth();

    ImGui::Text("Radius:");
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::SliderFloat("##Radius", &settings_->get_radius(), 0.1f, 200.f);
    ImGui::PopItemWidth();

    ImGui::Text("Signal X Scale:");
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::SliderFloat("##XScale", &settings_->get_x_scale(), 0.1f, 5.f);
    ImGui::PopItemWidth();

    ImGui::Text("Signal Y Scale:");
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::SliderFloat("##YScale", &settings_->get_y_scale(), 0.1f, 5.f);
    ImGui::PopItemWidth();
}
