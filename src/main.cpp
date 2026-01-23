
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include "../include/math/vector_math.hpp"
#include "../include/camera.hpp"
#include "../include/shader.hpp"
#include "../include/mesh.hpp"
#include "../include/window.hpp"
#include "../include/chunk.hpp"
#include "../include/object.hpp"
#include "../include/texture.hpp"
#include "../include/crosshair.hpp"


const int WIDTH = 800;
const int HEIGHT = 800;
const std::string WINDOW_TITLE = "GRAPHICS WINDOW";

int main() {

    /* initial setup */
    Window window(WIDTH,HEIGHT,WINDOW_TITLE);



    Shader main_program("shaders/mesh_shader_vertex.glsl","shaders/mesh_shader_fragment.glsl");
    /* transforms */
    vecmath::Vector3 camera_position(3.0f,-1.8f,3.0f);
    Camera camera(camera_position);
    vecmath::Matrix44 mat;
    main_program.setMat4("model",mat);
    main_program.setMat4("perspective",camera.get_perspective());
    main_program.setMat4("view",camera.get_view());

    /* chunk creation */
    const std::string current_chunk_save("savedata/chunk00.txt");
    const int bounds = 8;
    std::vector<Chunk> world;
    for (int i = 0; i < bounds; i ++) {
        for (int j = 0; j < bounds; j ++) {
            world.emplace_back(Chunk(current_chunk_save, (float)i, 0, (float)j)); // what is the safest cast here?
        }
    }

    // cleaner setup
    std::vector<std::string> file_list {"textures/stone_block.png","textures/ice_block.png","textures/dirt_block.png","textures/stone_block.png"};
    Texture chunk_texture(GL_TEXTURE_2D_ARRAY, file_list, 32, 32, 1);

    /* gl properties */
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    bool toggle = true;

    Crosshair crosshair;
    Shader gui_program("shaders/gui_shader_vertex.glsl", "shaders/gui_shader_fragment.glsl");
    std::vector<std::string> crosshair_textures {"textures/cross.png"};
    Texture crosshair_tex(GL_TEXTURE_2D_ARRAY, crosshair_textures, 32, 32, 0);

    camera.emitRay();


    /* main loop */
    while (window.notClosed()) {
        window.clearScreen();
        camera.update(window.getRawWindow());

        /* chunk drawing */
        chunk_texture.bindToTextureUnit(1);
        main_program.setInt("array_sampler", 1);
        main_program.setMat4("view",camera.get_view());
        main_program.use_program();
        for (auto & chunk : world) {
            chunk.reconstruct();
            chunk.draw();
        }

        /* crosshair drawing */
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
        glClear(GL_DEPTH_BUFFER_BIT);
        glClearDepth(1.0f);
        crosshair_tex.bindToTextureUnit(0);
        gui_program.setInt("sampler", 0);
        gui_program.use_program();
        crosshair.draw();
        glDisable(GL_BLEND);

        /* swap */
        window.pollEvents();
        window.swapBuffers();
    }
    return 0;
}