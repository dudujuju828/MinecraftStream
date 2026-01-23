
#include <glad/glad.h>
#include "../include/mesh.hpp"
#include "../include/object.hpp"
#include "../include/math/vector_math.hpp"
#include <string_view>
#include <string>

Object::Object(std::string filepath, GLuint program) : mesh{filepath}, program_id{program} {}

void Object::setPosition(vecmath::Vector3 position) {
    transform.position = position;
}

void Object::updateModelMatrix() {
    model.set_column(vecmath::Vector4(transform.position, 1.0f),3);
}

vecmath::Matrix44 Object::getModelMatrix() const {
    return model;
}

void Object::bind() const {
    glBindVertexArray(mesh.getVertexArrayID());
    glUseProgram(program_id);
}

void Object::draw() const {
    glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, mesh.indices.data());

    glBindVertexArray(0);
    glUseProgram(0);
}
