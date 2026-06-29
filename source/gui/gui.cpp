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
#include "renderers/wave_renderer.hpp"
#include "waves/wave_variant.hpp"

GUI::GUI() = default;

GUI::~GUI() = default;

namespace
{
    [[nodiscard]] auto ui_color(const int r, const int g, const int b, const int a = 255) -> ImVec4
    {
        return {static_cast<float>(r) / 255.f, static_cast<float>(g) / 255.f, static_cast<float>(b) / 255.f,
            static_cast<float>(a) / 255.f};
    }

    auto apply_imgui_theme() -> void
    {
        auto& style = ImGui::GetStyle();
        style.WindowPadding = {14.f, 14.f};
        style.FramePadding = {10.f, 6.f};
        style.ItemSpacing = {10.f, 8.f};
        style.ItemInnerSpacing = {8.f, 6.f};
        style.WindowRounding = 0.f;
        style.FrameRounding = 5.f;
        style.GrabRounding = 5.f;
        style.ScrollbarRounding = 5.f;
        style.WindowBorderSize = 0.f;
        style.FrameBorderSize = 1.f;

        auto& colors = style.Colors;
        colors[ImGuiCol_Text] = ui_color(220, 227, 234);
        colors[ImGuiCol_TextDisabled] = ui_color(139, 150, 163);
        colors[ImGuiCol_WindowBg] = ui_color(20, 24, 29, 245);
        colors[ImGuiCol_Border] = ui_color(55, 64, 74);
        colors[ImGuiCol_FrameBg] = ui_color(28, 34, 40);
        colors[ImGuiCol_FrameBgHovered] = ui_color(36, 43, 51);
        colors[ImGuiCol_FrameBgActive] = ui_color(43, 51, 60);
        colors[ImGuiCol_TitleBg] = ui_color(20, 24, 29);
        colors[ImGuiCol_TitleBgActive] = ui_color(20, 24, 29);
        colors[ImGuiCol_Button] = ui_color(38, 46, 54);
        colors[ImGuiCol_ButtonHovered] = ui_color(48, 58, 68);
        colors[ImGuiCol_ButtonActive] = ui_color(55, 67, 78);
        colors[ImGuiCol_Header] = ui_color(35, 45, 52);
        colors[ImGuiCol_HeaderHovered] = ui_color(46, 60, 68);
        colors[ImGuiCol_HeaderActive] = ui_color(52, 70, 78);
        colors[ImGuiCol_CheckMark] = ui_color(54, 209, 196);
        colors[ImGuiCol_SliderGrab] = ui_color(54, 209, 196);
        colors[ImGuiCol_SliderGrabActive] = ui_color(143, 211, 255);
    }

    auto push_button_colors(const ImVec4 color, const ImVec4 hovered, const ImVec4 active) -> void
    {
        ImGui::PushStyleColor(ImGuiCol_Button, color);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hovered);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, active);
    }

    auto draw_section_header(const char* label) -> void
    {
        ImGui::Spacing();
        ImGui::TextColored(ui_color(143, 211, 255), "%s", label);
        ImGui::Separator();
    }

    auto push_full_item_width() -> void
    {
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    }
} // namespace

auto GUI::initialize() -> void
{
    InitWindow(Config::Window::screen_width, Config::Window::screen_height, "Fourier Series");
    SetTargetFPS(Config::Window::target_fps);
    rlImGuiSetup(true);
    apply_imgui_theme();
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
    const auto screen_height = static_cast<float>(GetScreenHeight());
    ImGui::SetNextWindowPos({0.f, 0.f}, ImGuiCond_Always);
    ImGui::SetNextWindowSize({Config::Layout::settings_panel_width, screen_height}, ImGuiCond_Always);
    ImGui::Begin("Settings", nullptr,
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoTitleBar);
    update_settings();
    const auto panel_right = ImGui::GetWindowPos().x + ImGui::GetWindowWidth();
    ImGui::End();
    drawing_input_->handle();
    chart_renderer_->draw();
    wave_renderer_->draw();
    formula_renderer_->draw(panel_right);
    DrawLineV({panel_right, 0.f}, {panel_right, screen_height}, Config::Layout::canvas_divider);
    rlImGuiEnd();
    if (!settings_.get_is_paused())
        settings_.set_phase(settings_.get_phase() + settings_.frequency() * GetFrameTime());
}

auto GUI::update_settings() -> void
{
    settings_.set_fps(GetFPS());

    ImGui::TextColored(ui_color(220, 227, 234), "Fourier Series");
    ImGui::TextDisabled("FPS %d", settings_.get_fps());
    ImGui::SameLine();
    if (settings_.get_is_paused())
        ImGui::TextColored(ui_color(255, 209, 102), "Paused");
    else
        ImGui::TextColored(ui_color(54, 209, 196), "Running");

    draw_section_header("Playback");
    const auto button_width = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x) * 0.5f;
    if (settings_.get_is_paused())
    {
        push_button_colors(ui_color(32, 124, 118), ui_color(42, 148, 140), ui_color(50, 172, 162));
        if (ImGui::Button("Continue", {button_width, 0.f}))
            settings_.set_is_paused(false);
        ImGui::PopStyleColor(3);
    }
    else
    {
        if (ImGui::Button("Pause", {button_width, 0.f}))
            settings_.set_is_paused(true);
    }

    ImGui::SameLine();
    const auto can_clear = !settings_.wave().empty() || !settings_.path().empty();
    ImGui::BeginDisabled(!can_clear);
    if (ImGui::Button("Clear", {button_width, 0.f}))
    {
        settings_.wave().clear();
        settings_.path().clear();
    }
    ImGui::EndDisabled();

    ImGui::Text("Frequency");
    push_full_item_width();
    ImGui::SliderFloat("##Frequency", &settings_.mutable_frequency(), 0.f, 2.f, "%.2f");
    ImGui::PopItemWidth();

    draw_section_header("Wave");
    {
        auto selected_wave_index = static_cast<int>(settings_.get_selected_wave_index());
        ImGui::Text("Type");
        push_full_item_width();
        if (ImGui::Combo("##WaveType", &selected_wave_index, Waves::wave_names_cstr.data(), Waves::wave_names_cstr.size()))
            settings_.set_selected_wave_index(static_cast<std::size_t>(selected_wave_index));

        ImGui::PopItemWidth();
    }

    static constexpr auto number_of_harmonic_min = 1u;
    static constexpr auto number_of_harmonic_max = 100u;
    ImGui::Text("Harmonics");
    push_full_item_width();
    ImGui::SliderScalar("##NumberOfHarmonic", ImGuiDataType_U32, &settings_.mutable_number_of_harmonic(), &number_of_harmonic_min,
        &number_of_harmonic_max, "%u");
    ImGui::PopItemWidth();

    ImGui::Text("Radius");
    push_full_item_width();
    ImGui::SliderFloat("##Radius", &settings_.mutable_radius(), 0.1f, 200.f, "%.1f");
    ImGui::PopItemWidth();

    draw_section_header("Signal Scale");
    ImGui::Text("X");
    push_full_item_width();
    ImGui::SliderFloat("##XScale", &settings_.mutable_x_scale(), 0.1f, 5.f, "%.2f");
    ImGui::PopItemWidth();

    ImGui::Text("Y");
    push_full_item_width();
    ImGui::SliderFloat("##YScale", &settings_.mutable_y_scale(), 0.1f, 5.f, "%.2f");
    ImGui::PopItemWidth();
}
