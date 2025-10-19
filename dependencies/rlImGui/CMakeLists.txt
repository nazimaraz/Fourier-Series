cmake_minimum_required(VERSION 3.25)

add_library(rlImGui STATIC rlImGui/rlImGui.cpp)
target_include_directories(rlImGui PUBLIC rlImGui)
target_link_libraries(rlImGui PUBLIC raylib imgui)
