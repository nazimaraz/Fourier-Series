CollectSourceFiles(${CMAKE_CURRENT_SOURCE_DIR} source_files)
add_executable(${PROJECT_NAME} ${source_files})
target_link_libraries(${PROJECT_NAME} PUBLIC
        Boost::boost
        rlImGui
)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
