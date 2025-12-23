#pragma once

#include "../include/mesh.hpp"
#include "../include/math/vector_math.hpp"

#include <string_view>
#include <string>

class Object {
private:
    struct Transform {
        Transform() = default;
        vecmath::Vector3 position;
    };

public:
    Object(std::string filepath, GLuint program);
    void setPosition(vecmath::Vector3 position); 
    void updateModelMatrix();
    void draw() const;
    void bind() const;
    vecmath::Matrix44 getModelMatrix() const;

    vecmath::Matrix44 model;
    GLuint program_id;
    Mesh mesh;
    Transform transform;


};