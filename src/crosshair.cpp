

#include <glad/glad.h>
#include <spdlog/spdlog.h>

#include "../include/crosshair.hpp"

Crosshair::Crosshair() {
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    /* upload data here */
    glBufferData(GL_ARRAY_BUFFER, quad_data.size()*sizeof(float), quad_data.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    /* pointers when called VertexAttrib... use the currently bound buffer object */
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float),(void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    spdlog::info("Crosshair data generated.");
}

void Crosshair::draw() const {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}