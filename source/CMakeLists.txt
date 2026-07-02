set(fourier_core_sources
        waves/dft.cpp
        waves/formula_tex.cpp
        waves/wave_variant.cpp
        waves/shapes/ataturk_signature.cpp
        waves/shapes/cosine.cpp
        waves/shapes/full_wave_rectified_sine.cpp
        waves/shapes/half_wave_rectified_sine.cpp
        waves/shapes/heart.cpp
        waves/shapes/parabolic.cpp
        waves/shapes/sawtooth.cpp
        waves/shapes/semicircle.cpp
        waves/shapes/square.cpp
        waves/shapes/triangle.cpp
        waves/shapes/user_path.cpp
)
set(fourier_app_sources
        main.cpp
        gui/drawing_input.cpp
        gui/gui.cpp
        gui/settings.cpp
        renderers/chart_renderer.cpp
        renderers/formula_renderer.cpp
        renderers/wave_renderer.cpp
)
add_library(fourier_core ${fourier_core_sources})
target_compile_features(fourier_core PUBLIC cxx_std_23)
target_include_directories(fourier_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(fourier_core PUBLIC
        Boost::boost
        imgui
        raylib
)
CompileOptions(fourier_core)
add_executable(${PROJECT_NAME} ${fourier_app_sources})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE
        fourier_core
        rlImGui
        texrender::texrender
)
CompileOptions(${PROJECT_NAME})
texrender_copy_resources(${PROJECT_NAME})
