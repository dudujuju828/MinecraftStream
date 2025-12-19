
#include <glad/glad.h>
#include "../include/mesh.hpp"
#include <spdlog/spdlog.h>

#include <vector>
#include <fstream>
#include <iostream>

#include <string>
#include <stdexcept>

//void Mesh::drawMesh(int program);

/*
1. Converts string_input to float
2. Adds float to specific array dependent on last_type (v, vn, or vt)
*/
void Mesh::add_to_a_buffer(const std::string &string_input, const std::string &last_type) {
    try {
        float value = std::stof(string_input);
        if (last_type == "v") {
            vertices.push_back(value);
        } else if (last_type == "vn") {
            normals.push_back(value);
        } else if (last_type == "vt") {
            texture_coords.push_back(value);
        } 
    } catch (const std::exception &e) {
            spdlog::warn("invalid: {}",e.what());
    }
}

void Mesh::setup_buffers(const std::string &f_name) {
    std::ifstream file(f_name.c_str());
    if (file.is_open()) {
        spdlog::warn("Failed to open file: ", f_name.c_str());
    }

    std::string line_i;
    std::string last_type;
    while (std::getline(file,line_i,' ')) {
        std::size_t newline_location = line_i.find("\n");

        if (newline_location != std::string::npos) {
            /*A NEWLINE*/
            try {
                std::string t = line_i.substr(newline_location + 1);
                std::string value = line_i.substr(0,newline_location);
                add_to_a_buffer(value,last_type);
                last_type = t;
            } catch (const std::exception &e) {
                spdlog::warn("out of bounds, {}",e.what());
            }

        } else {
            /* NOT A NEW LINE*/
            add_to_a_buffer(line_i,last_type);
       }
    }
}

void Mesh::create_gl_buffers() {
    glGenBuffers(1,&vertex_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER,vertex_buffer_id);
    glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(float),vertices.data(),GL_STATIC_DRAW);

    glGenBuffers(1,&texture_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER,texture_buffer_id);
    glBufferData(GL_ARRAY_BUFFER,texture_coords.size()*sizeof(float),texture_coords.data(),GL_STATIC_DRAW);

    glGenBuffers(1,&normal_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER,normal_buffer_id);
    glBufferData(GL_ARRAY_BUFFER,normals.size()*sizeof(float),normals.data(),GL_STATIC_DRAW);

    glGenVertexArrays(1,&vertex_array_id);
    glBindVertexArray(vertex_array_id);

    glBindBuffer(GL_ARRAY_BUFFER,vertex_buffer_id);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);

    glBindBuffer(GL_ARRAY_BUFFER,normal_buffer_id);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);

    glBindBuffer(GL_ARRAY_BUFFER,texture_buffer_id);
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,2*sizeof(float),(void*)0);
   
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
}

/*
1. Reads in obj_file and updates internal buffers with vertex, position, normal, and texture data.
*/
Mesh::Mesh(std::string obj_file) {
    setup_buffers(obj_file);
    create_gl_buffers();
}