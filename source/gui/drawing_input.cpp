//
// Created by nazim on 5/26/26.
//

#include <algorithm>
#include <cmath>
#include <utility>
#include <variant>
#include <vector>
#include <imgui.h>
#include <raylib.h>
#include "drawing_input.hpp"
#include "config.hpp"
#include "settings.hpp"
#include "waves/dft.hpp"
#include "waves/shapes/user_path.hpp"
#include "waves/wave_variant.hpp"

using namespace UI;

DrawingInput::DrawingInput(Settings& settings)
    : settings_{settings}
{}

DrawingInput::~DrawingInput() = default;

void DrawingInput::handle() const
{
    if (!std::holds_alternative<Waves::UserPath>(settings_.get_selected_wave()))
        return;

    if (ImGui::GetIO().WantCaptureMouse)
        return;

    const auto mouse = raylib::GetMousePosition();
    const auto math_p = raylib::Vector2{mouse.x - Config::Wave::epicycle_origin_x, Config::Wave::epicycle_origin_y - mouse.y};
    if (raylib::IsMouseButtonPressed(raylib::MouseButton::MOUSE_BUTTON_LEFT))
        settings_.start_drawing();

    if (settings_.is_drawing() && raylib::IsMouseButtonDown(raylib::MouseButton::MOUSE_BUTTON_LEFT))
        settings_.add_drawing_point(math_p);

    if (settings_.is_drawing() && raylib::IsMouseButtonReleased(raylib::MouseButton::MOUSE_BUTTON_LEFT))
        commit();
}

void DrawingInput::commit() const
{
    settings_.stop_drawing();
    auto points = settings_.get_drawing_points();
    if (points.size() < 3)
        return;

    auto cx = 0.f;
    auto cy = 0.f;
    for (const auto& p : points)
    {
        cx += p.x;
        cy += p.y;
    }

    cx /= static_cast<float>(points.size());
    cy /= static_cast<float>(points.size());
    for (auto& p : points)
    {
        p.x -= cx;
        p.y -= cy;
    }

    auto max_dist = 0.f;
    for (const auto& p : points)
        max_dist = std::max(max_dist, std::hypot(p.x, p.y));

    if (max_dist > 0.f)
        settings_.set_radius(max_dist);

    auto [harmonics, dc_value] = Waves::compute_dft(points, Settings::max_harmonic_count);
    settings_.set_drawing_points(std::move(points));
    settings_.set_selected_wave(Waves::UserPath{.harmonics = std::move(harmonics), .dc_value = dc_value});
}
