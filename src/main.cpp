
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


const int WIDTH = 800;
const int HEIGHT = 800;
const std::string WINDOW_TITLE = "GRAPHICS WINDOW";

int main() {

    /* initial setup */
    Window window(WIDTH,HEIGHT,WINDOW_TITLE);
    Shader program_object("shaders/mesh_shader_vertex.glsl","shaders/mesh_shader_fragment.glsl");

    /* transforms */
    vecmath::Vector3 camera_position(0.0f,0.0f,-7.0f);
    Camera camera(camera_position);
    vecmath::Matrix44 mat;
    float scale = 1.0f;
    program_object.use_program();
    program_object.setMat4("model",mat);
    program_object.setMat4("perspective",camera.get_perspective());
    program_object.setMat4("view",camera.get_view());

    /* chunk creation */
    //vecmath::Vector3 chunk_position(0,0,0);
    //Chunk chunk("savedata/chunk00.txt", chunk_position);
    //chunk.reconstruct();
    const std::string current_chunk_save("savedata/chunk00.txt");
    const int bounds = 8;
    std::vector<Chunk> world;
    for (int i = 0; i < bounds; i ++) {
        for (int j = 0; j < bounds; j ++) {
            world.emplace(Chunk(current_chunk_save, i, 0, j));
        }
    }


    // cleaner setup
    std::vector<std::string> file_list {"textures/redstone_block.png","textures/ice_block.png","textures/dirt_block.png","textures/stone_block.png"};
    Texture texture_object(GL_TEXTURE_2D_ARRAY, file_list, 32, 32);
    texture_object.setActiveTextureUnit(program_object, "array_sampler", 0);

    /* gl properties */
    glEnable(GL_DEPTH_TEST);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    bool toggle = true;


    int x = 0.0f;
    int y = 0.0f;
    int z = 3.0f;


    /* main loop */
    while (window.notClosed()) {
        window.clearScreen();

        /* chunk drawing */
        program_object.use_program();
        chunk.reconstruct();
        chunk.draw();
        camera.update(window.getRawWindow());
        program_object.setMat4("view",camera.get_view());



        if (glfwGetKey(window.getRawWindow(), GLFW_KEY_P)) {
            if (toggle) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            toggle = !toggle;
        }
      
        if (glfwGetKey(window.getRawWindow(), GLFW_KEY_G)) {
            for(auto i : {0,10}) {
                x+=i;
                chunk.destroyBlock(x,y,z);
            }
        }
      

        window.pollEvents();
        window.swapBuffers();
    }
    return 0;
}