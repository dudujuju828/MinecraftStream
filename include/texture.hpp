


#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../include/shader.hpp"


class Texture {
public:

    Texture(GLenum type, std::vector<string> file_list, int image_width, int image_height); 
    void setActiveTextureUnit(Shader& s, std::string uniform_name, int tex_unit);

private:
    GLenum type;
    GLuint tex:
    GLuint tex_unit;
    int width;
    int height;


};