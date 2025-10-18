//
// Created by nazim on 18/10/2025.
//

#include <string>
#include <raylib.h>
#include "math/math.h"

void Setup();
void Update();

auto t = 0.f;

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

    auto radius = 100.f;
    raylib::DrawCircleLinesV({200, 200}, radius, raylib::RAYWHITE);

    auto x = radius * math::cos(t);
    auto y = radius * math::sin(t);
    t += 0.01f;
}
