//
// Created by nazim on 18/10/2025.
//

#include <boost/circular_buffer.hpp>
#include <raylib.h>
#include <imgui.h>
#include <rlImGui.h>
#include "Function.h"
#include "math/math.h"

void Setup();
void Update();
std::pair<int, float> GetFormula(int i);

auto t = 0.f;
auto speed = 0.02f;
auto wave = boost::circular_buffer<float>{1000};
auto N = 7;
auto current_function = Function::SQUARE_WAVE;
auto constant_radius = 75.f;

int main()
{
    constexpr auto screen_width = 1600;
    constexpr auto screen_height = 900;
    raylib::InitWindow(screen_width, screen_height, "Fourier Series");
    raylib::SetTargetFPS(60);
    raylib::rlImGuiSetup(true);
    Setup();
    while (!raylib::WindowShouldClose())
    {
        raylib::BeginDrawing();
        Update();
        raylib::EndDrawing();
    }

    raylib::CloseWindow();
    return 0;
}

void Setup()
{}

void Update()
{
    raylib::ClearBackground(raylib::BLACK);
    raylib::rlImGuiBegin();

    ImGui::SetNextWindowSizeConstraints(
    { 200.f, static_cast<float>(raylib::GetScreenHeight()) },
    { static_cast<float>(raylib::GetScreenWidth()), static_cast<float>(raylib::GetScreenHeight()) }
    );
    ImGui::SetNextWindowPos({0.f, 0.f});
    ImGui::Begin("Main Content", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
    ImGui::Text("FPS: %d", raylib::GetFPS());
    if (ImGui::Button("Clear"))
        wave.clear();

    static auto const function_names = std::vector{"Square Wave", "Sawtooth Wave", "Triangle Wave", "Semicircle"};
    auto temp_current_function = std::to_underlying(current_function);
    ImGui::Combo("Function", &temp_current_function, function_names.data(), function_names.size());
    current_function = static_cast<Function>(temp_current_function);
    ImGui::SliderInt("N", &N, 0, 100);
    ImGui::SliderFloat("f", &speed, 0, 0.15);
    ImGui::SliderFloat("Radius", &constant_radius, 0.1f, 200.f);
    ImGui::End();

    // ImGui::ShowDemoWindow();
    raylib::rlImGuiEnd();

    auto translate = raylib::Vector2{600, 450};
    auto position = raylib::Vector2{0, 0};
    for (auto i = 0; i < N; ++i)
    {
        const auto [n, radius] = GetFormula(i);
        raylib::DrawCircleLinesV(translate + position, radius, raylib::Color{255, 255, 255, 100});
        const auto previous_position = position;
        position += {
            radius * math::cos(n * t),
            radius * math::sin(n * t)
        };
        raylib::DrawLineV(translate + previous_position, translate + position, raylib::RAYWHITE);
    }

    wave.push_front(position.y);
    translate += { 200, 0 };
    raylib::DrawLineV(translate + position - raylib::Vector2{200, 0}, translate + raylib::Vector2{0, wave.front()}, raylib::RAYWHITE);
    std::vector<raylib::Vector2> points;
    for (auto i = 0; i < wave.size(); ++i)
        points.emplace_back(translate.x + i, translate.y + wave.at(i));

    raylib::DrawLineStrip(points, raylib::WHITE);
    t += speed;
}

std::pair<int, float> GetFormula(const int i)
{
    switch (current_function)
    {
        using enum Function;
        case SQUARE_WAVE:
        {
            const auto n = i * 2 + 1;
            const auto coeff = 4.f / (n * math::pi_v<float>);
            const auto radius = constant_radius * (coeff / (4.f / math::pi_v<float>));
            // radius = constant_radius * (4 / (n * math::pi_v<float>));
            return { n, radius };
        }

        case SAWTOOTH_WAVE:
        {
            const auto n = i + 1;
            const auto coeff = 2.f / (n * math::pi_v<float>);
            const auto radius = 0.5f - constant_radius * (coeff / (2.f / math::pi_v<float>));
            // radius = constant_radius * (1 / (n * math::pi_v<float>));
            return { n, radius };
        }

        case TRIANGLE_WAVE:
        {
            break;
        }

        case SEMICIRCLE:
        {
            break;
        }

        default:
        break;
    }

    return {};
}
