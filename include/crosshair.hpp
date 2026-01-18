
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
    const float crossHairSize = 2.0f;

};