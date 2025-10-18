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

    auto translate = raylib::Vector2{150, 200};
    auto x = 0.f;
    auto y = 0.f;
    for (auto i = 0; i < 2; ++i)
    {
        auto prev_x = x;
        auto prev_y = y;
        auto n = i * 2 + 1;
        auto radius = 75 * (4 / (n * math::pi_v<float>));
        x += radius * math::cos(n * t);
        y += radius * math::sin(n * t);

        raylib::DrawCircleLinesV({translate.x + prev_x, translate.y + prev_y}, radius, raylib::Color{255, 255, 255, 100});
        raylib::DrawLine(translate.x + prev_x, translate.y + prev_y, translate.x + x, translate.y + y, raylib::RAYWHITE);
        raylib::DrawCircleV({ translate.x + x, translate.y + y}, 8, raylib::RAYWHITE);
    }

    wave.insert(wave.begin(), y);
    translate = { translate.x + 200, translate.y + 0 };
    raylib::DrawLine(translate.x + x - 200, translate.y + y, translate.x + 0, translate.y + wave[0], raylib::RAYWHITE);
    std::vector<raylib::Vector2> points;
    for (auto i = 0; i < wave.size(); i++)
        points.push_back({translate.x + i, translate.y + wave[i]});

    raylib::DrawLineStrip(points.data(), points.size(), raylib::WHITE);
    t += 0.03f;
    if (wave.size() > 1000)
        wave.pop_back();
}
