
cmake_minimum_required(VERSION 3.20)
project(minecraft)
# REVIEW: consider CMAKE_CXX_STANDARD 20 — the code uses C++17 features like
# string_view and init-statements in if, but C++20 adds useful features like
# std::format and concepts that could help clean up the math library.
set(CMAKE_CXX_STANDARD 17)

set(GLAD_DIR ${CMAKE_SOURCE_DIR}/external/glad/)
set(STB_DIR ${CMAKE_SOURCE_DIR}/external/stb/)
# REVIEW: source files are listed manually — adding a new .cpp requires editing this line.
# Consider using a GLOB or listing files via a variable for easier maintenance.
# Also, world.cpp, engine.cpp, renderer.cpp, game.cpp are not included — they're empty
# but if they get code added, they'll be silently excluded from the build.
# The pre-optimal/ files (mesh.cpp, object.cpp) are also excluded from the build.
add_executable(main src/main.cpp ${GLAD_DIR}/src/glad.c src/camera.cpp src/shader.cpp src/window.cpp src/chunk.cpp src/texture.cpp src/vertex.cpp src/crosshair.cpp)

include(FetchContent)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.16.0
)


FetchContent_MakeAvailable(glfw spdlog)

# REVIEW: consider adding target_compile_options for warnings:
#   target_compile_options(main PRIVATE -Wall -Wextra -Wpedantic)
# This would catch many of the issues identified in this review at compile time.
target_link_libraries(main PRIVATE glfw spdlog)
target_include_directories(main PRIVATE ${GLAD_DIR}/include/ ${STB_DIR}/include/)