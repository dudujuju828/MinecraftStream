

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <cstdlib>
#include "../include/window.hpp"

static void window_callback(GLFWwindow* window, int width, int height) {
    glViewport(0,0,width,height);
}

/* CAN ONLY USE 1 WINDOW ATM */
Window::Window(int width, int height, std::string name) : window_name{name} {
    if (!glfwInit()) {
        spdlog::error("GLFW not initialize.");
        exit(-1);
    } else {
        spdlog::info("Successfully initailized GLFW.");
    }

    raw_window = glfwCreateWindow(width, height, name.data(), nullptr, nullptr);
    if (!raw_window) {
        spdlog::error("Window was not created.");
        exit(-1);
    } else {
        spdlog::info("Successfully created window.");
    }

    glfwMakeContextCurrent(raw_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        spdlog::error("Error loading OpenGL function pointers.");
        exit(-1);
    } 
    spdlog::info("OpenGL function pointers loaded in successfully.");

    glfwSetInputMode(raw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetWindowSizeCallback(raw_window,window_callback);

}

GLFWwindow* Window::getRawWindow() {
    return raw_window;
}

bool Window::notClosed() const {
    return !glfwWindowShouldClose(raw_window);
}
 
void Window::swapBuffers() const {
    glfwSwapBuffers(raw_window);
}
 
void Window::pollEvents() const {
    glfwPollEvents();
    if (glfwGetKey(raw_window,GLFW_KEY_ESCAPE)==GLFW_PRESS) {
        glfwSetWindowShouldClose(raw_window,GLFW_TRUE);
    }
}
void Window::clearScreen() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f,0.8f,1.0f,1.0f);
}

Window::~Window() {
    glfwDestroyWindow(raw_window);
    spdlog::warn("Window named: {} has been destroyed.", window_name.c_str());
    
    glfwTerminate();
    spdlog::warn("GLFW has been terminated.");
}