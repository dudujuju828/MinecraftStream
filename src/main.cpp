
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

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

const int WIDTH = 800;
const int HEIGHT = 800;
const std::string WINDOW_TITLE = "GRAPHICS WINDOW";


int main() {

    Window window(WIDTH,HEIGHT,WINDOW_TITLE);
    Shader program_object("shaders/mesh_shader_vertex.glsl","shaders/mesh_shader_fragment.glsl");
    Object obj("models/cube.obj", program_object.get_program_id());

    vecmath::Vector3 camera_position(0.0f,0.0f,-7.0f);
    Camera camera(camera_position);


    vecmath::Matrix44 mat;
   
    float scale = 1.0f;

    program_object.use_program();
    program_object.setMat4("model",mat);
    program_object.setMat4("perspective",camera.get_perspective());
    program_object.setMat4("view",camera.get_view());

    GLuint texture;
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    int width, height, nrChannel;
    unsigned char* image_data = stbi_load("textures/dirt_block.png",&width,&height,&nrChannel,0);
    // check image data exists (later)
    GLenum type;
    switch (nrChannel) {
        case 3:
            type = GL_RGB;
            break;
        case 4:
            type = GL_RGBA;
            break;
        default:
            type = GL_RGB;
            break;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    glGenerateMipmap(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    program_object.setInt("sampler",0);

    /*
    glfwSetCur 
    */
    Chunk chunk("savedata/chunk00.txt");
    chunk.print();

    program_object.setMat4("model",mat);

    /*FIRST OPTIMIZATION*/
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_FRONT);

    glEnable(GL_DEPTH_TEST);
    while (window.notClosed()) {
        window.clearScreen();

        program_object.use_program();


        camera.update(window.getRawWindow());
        program_object.setMat4("view",camera.get_view());


        for (int i = 0; i < chunk.SIZE; i++) {
            for (int j = 0; j < chunk.SIZE; j++) {
                for (int k = 0; k < chunk.SIZE; k++) {
                    obj.bind();

                    obj.setPosition(vecmath::Vector3(((float)i*2.0f) ,((float)j*2.0f),((float)k*2.0f)));
                    obj.updateModelMatrix();
                    program_object.setMat4("model",obj.getModelMatrix());

                    obj.draw();
                }
            }
        }
       

        window.pollEvents();
        window.swapBuffers();
    }
    return 0;
}