//
// Created by nazim on 18/10/2025.
//

#include <boost/circular_buffer.hpp>
#include <string>
#include <raylib.h>
#include "math/math.h"

void Setup();
void Update();

auto t = 0.f;
auto wave = boost::circular_buffer<float>{1000};

int main()
{
    constexpr auto screen_width = 800;
    constexpr auto screen_height = 450;
    raylib::InitWindow(screen_width, screen_height, "Fourier Series");
    raylib::SetTargetFPS(60);
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
    raylib::DrawText((std::string{"FPS: "} + std::to_string(raylib::GetFPS())).c_str(), 10, 10, 20, raylib::DARKGRAY);
    auto translate = raylib::Vector2{150, 200};
    auto position = raylib::Vector2{0, 0};
    for (auto i = 0; i < 3; ++i)
    {
        const auto n = i * 2 + 1;
        const auto previous = position;
        const auto radius = 75 * (4 / (n * math::pi_v<float>));
        position += {
            radius * math::cos(n * t),
            radius * math::sin(n * t)
        };
        raylib::DrawCircleLinesV(translate + previous, radius, raylib::Color{255, 255, 255, 100});
        raylib::DrawLineV(translate + previous, translate + position, raylib::RAYWHITE);
    }

    wave.push_front(position.y);
    translate += { 200, 0 };
    raylib::DrawLineV(translate + position - raylib::Vector2{200, 0}, translate + raylib::Vector2{0, wave.front()}, raylib::RAYWHITE);
    std::vector<raylib::Vector2> points;
    for (auto i = 0; i < wave.size(); ++i)
        points.emplace_back(translate.x + i, translate.y + wave.at(i));

    raylib::DrawLineStrip(points, raylib::WHITE);
    t += 0.03f;
}
