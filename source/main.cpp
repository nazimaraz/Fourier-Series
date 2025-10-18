//
// Created by nazim on 18/10/2025.
//

#include <string>
#include <raylib.h>
#include <vector>
#include "math/math.h"

void Setup();
void Update();

auto t = 0.f;
auto wave = std::vector<float>{};

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

    auto translate = raylib::Vector2{200, 200};
    auto radius = 100.f;
    raylib::DrawCircleLinesV({translate.x + 0, translate.y + 0}, radius, raylib::RAYWHITE);

    auto x = radius * math::cos(t);
    auto y = radius * math::sin(t);
    wave.insert(wave.begin(), y);
    raylib::DrawLine(translate.x + 0, translate.y + 0, translate.x + x, translate.y + y, raylib::RAYWHITE);
    raylib::DrawCircleV({ translate.x + x, translate.y + y}, 8, raylib::RAYWHITE);
    translate = { translate.x + 200, translate.y + 0 };
    raylib::DrawLine(translate.x + x - 200, translate.y + y, translate.x + 0, translate.y + wave[0], raylib::RAYWHITE);

    std::vector<raylib::Vector2> points;
    for (auto i = 0; i < wave.size(); i++)
        points.push_back({translate.x + i, translate.y + wave[i]});

    raylib::DrawLineStrip(points.data(), points.size(), raylib::WHITE);

    t += 0.05f;

    if (wave.size() > 1000)
        wave.pop_back();
}
