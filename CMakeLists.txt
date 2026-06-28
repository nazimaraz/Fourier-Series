cmake_minimum_required(VERSION 3.25)
project(Fourier-Series
        VERSION 0.1.0
        LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(CompileOptions)

if (EMSCRIPTEN)
    set(PLATFORM "Web" CACHE STRING "" FORCE)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
    set(OPENGL_VERSION "ES 2.0" CACHE STRING "" FORCE)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

    # Emscripten-specific linker/debug flags
    add_compile_options(-fexceptions "$<$<CONFIG:Debug>:-gsource-map>")
    add_link_options(
            -fexceptions
            -sDISABLE_EXCEPTION_CATCHING=0
            "$<$<CONFIG:Debug>:-gsource-map>"
            "$<$<CONFIG:Debug>:-sASSERTIONS=2>"
    )
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s WASM=1 -s ASYNCIFY=1")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s ALLOW_MEMORY_GROWTH=1 --shell-file ${CMAKE_SOURCE_DIR}/shell.html")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")

    # Raylib specific options for Web
    set(SUPPORT_DEFAULT_FONT ON CACHE BOOL "" FORCE)
    set(SUPPORT_SCREEN_CAPTURE ON CACHE BOOL "" FORCE)
    set(SUPPORT_GIF_RECORDING ON CACHE BOOL "" FORCE)
    set(SUPPORT_COMPRESSION_API ON CACHE BOOL "" FORCE)
endif ()

add_subdirectory(dependencies)
add_subdirectory(source)
