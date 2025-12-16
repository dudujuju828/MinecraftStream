
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include "../include/math/vector3.hpp"
#include "../include/math/vector4.hpp"
#include "../include/math/matrix44.hpp"
#include "../include/camera.hpp"

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

    /*
    OPENGL Specific 
    */


    std::vector<float> triangle_data {
        0.2f, -0.5f, 0.0f,
        -0.2f, -0.5f, 0.0f,
        0.0f, 0.0f, 0.0f
    };

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, triangle_data.size()*sizeof(float), triangle_data.data(), GL_STATIC_DRAW);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    std::stringstream buffer;
    std::ifstream vertex_file("shaders/triangle_vertex.glsl");
    std::ifstream fragment_file("shaders/triangle_fragment.glsl");

    buffer << vertex_file.rdbuf();
    std::string vertex_string = buffer.str();
    const char * vertex_source_cstyle = vertex_string.c_str();
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader,1,&vertex_source_cstyle, nullptr);
    glCompileShader(vertex_shader);

    buffer.str("");
    buffer << fragment_file.rdbuf();
    std::string fragment_string = buffer.str();
    const char * fragment_source_cstyle = fragment_string.c_str();
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader,1,&fragment_source_cstyle, nullptr);
    glCompileShader(fragment_shader);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);


    Vector3 camera_position(0.0f,0.0f,3.0f);
    Camera cam(camera_position);

    const int buffer_size = 16;
    float buf[buffer_size] = {1.0f, 0.0f, 0.0f, 0.0f,/* */ 0.0f, 1.0f, 0.0f, 0.0f,
                             0.0f, 0.0f, 1.0f, 0.0f, /* */0.0f, 0.0f, 0.0f, 1.0f};
    


    Matrix44 mat(buf,buffer_size);
    

    mat.scale(5.0f);
   

    glUseProgram(program);
    GLint model_location = glGetUniformLocation(program, "model");
    spdlog::info("model_location: {}",model_location);
    

    glUniformMatrix4fv(model_location, 1, GL_FALSE, mat.get_buf());
    
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0f,0.7f,0.7f,1.0f);

        glBindVertexArray(VAO);
        glUseProgram(program);
        glDrawArrays(GL_TRIANGLES, 0, 3);


        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    spdlog::warn("Window named: {} has been destroyed.", WINDOW_TITLE.c_str());
    
    glfwTerminate();
    spdlog::warn("GLFW has been terminated.");

    return 0;
}