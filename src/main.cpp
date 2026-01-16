
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
    vecmath::Vector3 chunk_position(0,0,1);
    Chunk chunk("savedata/chunk00.txt", chunk_position);
    std::vector<Vertex> chunk_vertex = chunk.constructMesh();
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    std::vector<float> flattened_data;
    for (auto &vertex : chunk_vertex) {
        /* potential problem with xyz xzy*/
        flattened_data.push_back(vertex.x);
        flattened_data.push_back(vertex.y);
        flattened_data.push_back(vertex.z);
        flattened_data.push_back(vertex.u);
        flattened_data.push_back(vertex.v);
        flattened_data.push_back(vertex.w);
    }
    glBufferData(GL_ARRAY_BUFFER, flattened_data.size() * sizeof(float), flattened_data.data(), GL_STATIC_DRAW);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    std::vector<std::string> file_list {"textures/ice_block.png","textures/dirt_block.png","textures/stone_block.png"};

    // cleaner setup
    Texture texture_object(GL_TEXTURE_2D_ARRAY, file_list, 32, 32);
    texture_object.setActiveTextureUnit(program_object, "array_sampler", 0);

    /* gl properties */
    glEnable(GL_DEPTH_TEST);

    glPointSize(5.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    bool toggle = true;

    /* main loop */
    while (window.notClosed()) {
        window.clearScreen();

        program_object.use_program();


        camera.update(window.getRawWindow());
        program_object.setMat4("view",camera.get_view());

        if (glfwGetKey(window.getRawWindow(), GLFW_KEY_P)) {
            if (toggle) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            toggle = !toggle;
        }

        /* chunk drawing */
        for (int i = 0; i < chunk.SIZE; i++) {
            for (int j = 0; j < chunk.SIZE; j++) {
                for (int k = 0; k < chunk.SIZE; k++) {
                    glBindVertexArray(VAO);
                    glDrawArrays(GL_TRIANGLES, 0, flattened_data.size()/6);
                }
            }
        }
       

        window.pollEvents();
        window.swapBuffers();
    }
    return 0;
}