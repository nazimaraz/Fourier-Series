//
// Created by Nazım Can on 25.10.2025.
//

#include <array>
#include <cstddef>
#include <cstdio>
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
        return {
            static_cast<float>(r) / 255.f, static_cast<float>(g) / 255.f, static_cast<float>(b) / 255.f,
            static_cast<float>(a) / 255.f
        };
    }

    auto apply_imgui_theme() -> void
    {
        namespace Theme = Config::Theme;

        auto& style = ImGui::GetStyle();
        style.WindowPadding = Theme::window_padding;
        style.FramePadding = Theme::frame_padding;
        style.ItemSpacing = Theme::item_spacing;
        style.ItemInnerSpacing = Theme::item_inner_spacing;
        style.WindowRounding = Theme::window_rounding;
        style.FrameRounding = Theme::frame_rounding;
        style.GrabRounding = Theme::grab_rounding;
        style.ScrollbarRounding = Theme::scrollbar_rounding;
        style.WindowBorderSize = Theme::window_border_size;
        style.FrameBorderSize = Theme::frame_border_size;

        auto& colors = style.Colors;
        colors[ImGuiCol_Text] = Theme::text_color;
        colors[ImGuiCol_TextDisabled] = Theme::text_disabled_color;
        colors[ImGuiCol_WindowBg] = Theme::window_background_color;
        colors[ImGuiCol_Border] = Theme::border_color;
        colors[ImGuiCol_FrameBg] = Theme::frame_background_color;
        colors[ImGuiCol_FrameBgHovered] = Theme::frame_background_hovered_color;
        colors[ImGuiCol_FrameBgActive] = Theme::frame_background_active_color;
        colors[ImGuiCol_TitleBg] = Theme::title_background_color;
        colors[ImGuiCol_TitleBgActive] = Theme::title_background_active_color;
        colors[ImGuiCol_Button] = Theme::button_color;
        colors[ImGuiCol_ButtonHovered] = Theme::button_hovered_color;
        colors[ImGuiCol_ButtonActive] = Theme::button_active_color;
        colors[ImGuiCol_Header] = Theme::header_color;
        colors[ImGuiCol_HeaderHovered] = Theme::header_hovered_color;
        colors[ImGuiCol_HeaderActive] = Theme::header_active_color;
        colors[ImGuiCol_CheckMark] = Theme::check_mark_color;
        colors[ImGuiCol_SliderGrab] = Theme::slider_grab_color;
        colors[ImGuiCol_SliderGrabActive] = Theme::slider_grab_active_color;
    }

    auto push_button_colors(const ImVec4 color, const ImVec4 hovered, const ImVec4 active) -> void
    {
        ImGui::PushStyleColor(ImGuiCol_Button, color);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hovered);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, active);
    }

    auto draw_section_header(const char* label) -> void
    {
        ImGui::Dummy({0.f, 6.f});
        ImGui::TextColored(ui_color(143, 211, 255), "%s", label);
        const auto start = ImGui::GetCursorScreenPos();
        const auto width = ImGui::GetContentRegionAvail().x;
        ImGui::GetWindowDrawList()->AddLine(start, {start.x + width, start.y}, ImGui::GetColorU32(ui_color(55, 64, 74)), 1.f);
        ImGui::Dummy({0.f, 5.f});
    }

    auto push_full_item_width() -> void
    {
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    }

    auto draw_control_label(const char* label, const char* value) -> void
    {
        const auto cursor_x = ImGui::GetCursorPosX();
        const auto available_width = ImGui::GetContentRegionAvail().x;
        const auto value_width = ImGui::CalcTextSize(value).x;
        ImGui::TextUnformatted(label);
        ImGui::SameLine();
        ImGui::SetCursorPosX(cursor_x + available_width - value_width);
        ImGui::TextDisabled("%s", value);
    }

    auto draw_float_slider(const char* label, const char* id, float& value, const float min, const float max, const char* format)
        -> void
    {
        auto value_text = std::array<char, 32>{};
        if (const auto written = std::snprintf(value_text.data(), value_text.size(), format, value);
            written < 0 || static_cast<std::size_t>(written) >= value_text.size())
            value_text.back() = '\0';

        draw_control_label(label, value_text.data());
        push_full_item_width();
        ImGui::SliderFloat(id, &value, min, max, format);
        ImGui::PopItemWidth();
    }

    auto draw_uint_slider(const char* label, const char* id, unsigned int& value, const unsigned int min, const unsigned int max)
        -> void
    {
        auto value_text = std::array<char, 32>{};
        if (const auto written = std::snprintf(value_text.data(), value_text.size(), "%u", value);
            written < 0 || static_cast<std::size_t>(written) >= value_text.size())
            value_text.back() = '\0';

        draw_control_label(label, value_text.data());
        push_full_item_width();
        ImGui::SliderScalar(id, ImGuiDataType_U32, &value, &min, &max, "%u");
        ImGui::PopItemWidth();
    }

    auto reset_visualization(UI::Settings& settings) -> void
    {
        settings.wave().clear();
        settings.path().clear();
        settings.set_phase(0.f);
        settings.set_number_of_harmonic(Config::Defaults::harmonic_count);
        settings.set_frequency(Config::Defaults::frequency);
        settings.set_radius(Config::Defaults::radius);
        settings.set_x_scale(Config::Defaults::x_scale);
        settings.set_y_scale(Config::Defaults::y_scale);
        settings.reset_harmonic_mask();
        settings.set_selected_wave_index(Waves::index_of<Waves::Square>);
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

    if (ImGui::Button("Reset", {ImGui::GetContentRegionAvail().x, 0.f}))
        reset_visualization(settings_);

    draw_float_slider("Frequency", "##Frequency", settings_.mutable_frequency(), 0.f, 2.f, "%.2f");

    draw_section_header("Wave");
    {
        auto selected_wave_index = static_cast<int>(settings_.get_selected_wave_index());
        ImGui::Text("Type");
        push_full_item_width();
        if (ImGui::Combo("##WaveType", &selected_wave_index, Waves::wave_names_cstr.data(), Waves::wave_names_cstr.size()))
            settings_.set_selected_wave_index(static_cast<std::size_t>(selected_wave_index));

        ImGui::PopItemWidth();
    }

    draw_uint_slider("Harmonics", "##NumberOfHarmonic", settings_.mutable_number_of_harmonic(), 1u,
        Config::Defaults::max_harmonic_count);
    draw_float_slider("Radius", "##Radius", settings_.mutable_radius(), 0.1f, 200.f, "%.1f");
    draw_section_header("Signal Scale");
    draw_float_slider("X", "##XScale", settings_.mutable_x_scale(), 0.1f, 5.f, "%.2f");
    draw_float_slider("Y", "##YScale", settings_.mutable_y_scale(), 0.1f, 5.f, "%.2f");
}
