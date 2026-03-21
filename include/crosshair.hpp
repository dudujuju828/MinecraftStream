
#pragma once

#include <glad/glad.h>

#include <vector>

#include "../include/vertex.hpp"


class Crosshair {
    public:
    Crosshair();
    const std::vector<float> quad_data {
        //     x    y    z    u    v    w
            -0.5f,-0.5f,0.5f,0.0f,0.0f,0.0f,
            0.5f,-0.5f,0.5f,1.0f,0.0f,0.0f,
            0.5f,0.5f,0.5f,1.0f,1.0f,0.0f,

            0.5f,0.5f,0.5f,1.0f,1.0f,0.0f,
            -0.5f,0.5f,0.5f,0.0f,1.0f,0.0f,
            -0.5f,-0.5f,0.5f,0.0f,0.0f,0.0f
    }; 

    void draw() const;


    private:

    GLuint VBO;
    GLuint VAO;
    // REVIEW: crossHairSize is declared but never used anywhere. The crosshair scale
    // is hardcoded as 0.18f in the gui_shader_vertex.glsl shader. Remove this or
    // pass it as a uniform to the shader for consistency.
    const float crossHairSize = 2.0f;

};