
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

/* currently: 1 cube, many-draw-calls per cube position*/
/* wanted: mesh, created depending on chunk*/
/* relationship: mesh depend on chunk */
/* process: when chunk loaded, upload chunk data to the gpu */
/* technically: building the mesh, calling glBufferData / subData */

struct Vertex {
    Vertex() = default;

    float x, y, z, u, v, w;
};
/* chunk[0][2][5] */



int main() {

    /* initial setup */
    Window window(WIDTH,HEIGHT,WINDOW_TITLE);
    Shader program_object("shaders/mesh_shader_vertex.glsl","shaders/mesh_shader_fragment.glsl");
    Object obj("models/cube.obj", program_object.get_program_id());

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
    Chunk chunk("savedata/chunk00.txt");
    chunk.print();


    /* 2d texture array*/
    GLuint tex;
    glGenTextures(1,&tex);
    glBindTexture(GL_TEXTURE_2D_ARRAY, tex);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,
                GL_TEXTURE_MIN_FILTER,
                GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,
                GL_TEXTURE_MAG_FILTER,
                GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
    const int tex_count = 5;
    const int image_width = 256;
    const int image_height = 256;
    std::vector<const char*> file_list = {
        "textures/blue.png",
        "textures/orange.png",
        "textures/red.png",
        "textures/green.png",
        "textures/brown.png",
    };
    std::vector<unsigned char*> image_ptr_list(tex_count,nullptr);
    int width, height, nrChannel, format;
    glBindTexture(GL_TEXTURE_2D_ARRAY, tex);
    glActiveTexture(GL_TEXTURE0);   
    unsigned char* image_data_second = stbi_load("textures/red.png",&width,&height,&nrChannel,0);
    if (image_data_second) {
        spdlog::warn("Data-loaded");
    }
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
    glTexImage3D(GL_TEXTURE_2D_ARRAY,0,type,image_width,image_height,tex_count,0,type,GL_UNSIGNED_BYTE,NULL);
    for (int i = 0; i < 5; i++) {
        int x, y;
        image_ptr_list.at(i) = stbi_load(file_list.at(i),&x,&y,&format,0);
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, image_width, image_height, 1, type, GL_UNSIGNED_BYTE, image_ptr_list.at(i));
    }
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
    program_object.setInt("array_sampler",0);

    /*raw cube data*/
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f
    };



    /* gl properties */
    glEnable(GL_DEPTH_TEST);

    /* main loop */
    while (window.notClosed()) {
        window.clearScreen();

        program_object.use_program();


        camera.update(window.getRawWindow());
        program_object.setMat4("view",camera.get_view());


        /* chunk drawing */
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