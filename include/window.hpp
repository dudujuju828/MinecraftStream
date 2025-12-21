#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class Window {
    public:
    Window(int width, int height, std::string name); 
    ~Window();
    void clearScreen() const;
    void pollEvents() const;
    void swapBuffers() const;
    bool notClosed() const;
    GLFWwindow* getRawWindow();


    private:
    std::string window_name;
    GLFWwindow* raw_window;


};