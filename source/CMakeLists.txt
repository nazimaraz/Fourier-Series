CollectSourceFiles(${CMAKE_CURRENT_SOURCE_DIR}/math core_math_sources)
CollectSourceFiles(${CMAKE_CURRENT_SOURCE_DIR}/waves core_wave_sources)
add_library(fourier_core
        ${core_math_sources}
        ${core_wave_sources}
)
target_include_directories(fourier_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(fourier_core PUBLIC
        Boost::boost
        raylib
)
CollectSourceFiles(${CMAKE_CURRENT_SOURCE_DIR}/gui app_gui_sources)
CollectSourceFiles(${CMAKE_CURRENT_SOURCE_DIR}/renderers app_renderer_sources)
add_executable(${PROJECT_NAME}
        main.cpp
        ${app_gui_sources}
        ${app_renderer_sources}
)
target_link_libraries(${PROJECT_NAME} PRIVATE
        fourier_core
        rlImGui
        texrender::texrender
)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
texrender_copy_resources(${PROJECT_NAME})
