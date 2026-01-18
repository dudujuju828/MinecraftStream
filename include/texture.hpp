


#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../include/shader.hpp"
#include <string>


class Texture {
public:

    Texture(GLenum type, std::vector<std::string> &file_list, int image_width, int image_height, int tex_unit); 
    void bindToTextureUnit(int tex_unit);

private:
    GLenum type;
    GLuint tex;
    GLuint tex_unit;
    int width;
    int height;


};