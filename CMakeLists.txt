
cmake_minimum_required(VERSION 3.20)
project(minecraft)
set(CMAKE_CXX_STANDARD 17)

set(GLAD_DIR ${CMAKE_SOURCE_DIR}/external/glad/)
set(STB_DIR ${CMAKE_SOURCE_DIR}/external/stb/)
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

target_link_libraries(main PRIVATE glfw spdlog)
target_include_directories(main PRIVATE ${GLAD_DIR}/include/ ${STB_DIR}/include/)