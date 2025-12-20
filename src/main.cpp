
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

const int WIDTH = 800;
const int HEIGHT = 800;
const std::string WINDOW_TITLE = "GRAPHICS WINDOW";


int main() {

    if (!glfwInit()) {
        spdlog::error("GLFW not initialize.");
        return -1;
    } else {
        spdlog::info("Successfully initailized GLFW.");
    }

    /*
    OPENGL Setup
    */
    GLFWwindow* window = glfwCreateWindow(WIDTH,HEIGHT,WINDOW_TITLE.c_str(), nullptr, nullptr);
    if (!window) {
        spdlog::error("Window was not created.");
        return -1;
    } else {
        spdlog::info("Successfully created window.");
    }

    glfwMakeContextCurrent(window);
    spdlog::info("Window named: {} has been made current context.", WINDOW_TITLE.c_str());

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        spdlog::error("Error loading OpenGL function pointers.");
        return -1;
    } 
    spdlog::info("OpenGL function pointers loaded in successfully.");

    Shader program_object("shaders/triangle_vertex.glsl","shaders/triangle_fragment.glsl");
    //Mesh mesh("models/cube.obj");
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    GLuint VBO;
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    GLuint VAO;
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)0);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3*sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);



    vecmath::Vector3 camera_position(0.0f,0.0f,3.0f);
    Camera camera(camera_position);


    vecmath::Matrix44 mat;
   
    float scale = 1.0f;

    program_object.use_program();
    GLint model_location = glGetUniformLocation(program_object.get_program_id(), "model");
    GLint perspective_location = glGetUniformLocation(program_object.get_program_id(), "perspective");
    GLint view_location = glGetUniformLocation(program_object.get_program_id(), "view");
    spdlog::info("model_location: {}",model_location);
    glUniformMatrix4fv(model_location, 1, GL_FALSE, mat.get_buf());
    glUniformMatrix4fv(perspective_location, 1, GL_FALSE, camera.get_perspective().get_buf());
    glUniformMatrix4fv(view_location, 1, GL_FALSE, camera.get_view().get_buf());


    /*
    
    MESH TEST 
    
    */


    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f,0.7f,0.7f,1.0f);
        program_object.use_program();
        model_location = glGetUniformLocation(program_object.get_program_id(), "model");
        glUniformMatrix4fv(model_location, 1, GL_FALSE, mat.get_buf());

        camera.poll_input(window);
        glUniformMatrix4fv(view_location, 1, GL_FALSE, camera.get_view().get_buf());

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,36);


        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    spdlog::warn("Window named: {} has been destroyed.", WINDOW_TITLE.c_str());
    
    glfwTerminate();
    spdlog::warn("GLFW has been terminated.");

    return 0;
}