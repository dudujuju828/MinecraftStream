
#include <glad/glad.h>
#include "../include/mesh.hpp"
#include <spdlog/spdlog.h>

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

#include <string>
#include <stdexcept>

void Mesh::drawMesh(GLuint program) {
    glBindVertexArray(vertex_array_id);
    glUseProgram(program);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());

    glBindVertexArray(0);
    glUseProgram(0);
}

/*
1. Converts string_input to float
2. Adds float to specific array dependent on last_type (v, vn, or vt)
*/
void Mesh::add_to_a_buffer(const std::string &string_input, const std::string &last_type, int& global_count, int& factor) {
    try {
        float value = std::stof(string_input);
        if (last_type == "v") {
            vertices.push_back(value);
        } else if (last_type == "vn") {
            normals.push_back(value);
        } else if (last_type == "vt") {
            texture_coords.push_back(value);
        } else if (last_type == "f") {
            /* split the string */
            size_t has_newline = string_input.find('\n'); 
            if (has_newline != std::string::npos) {
                spdlog::info("String has newline: {}", string_input);
            } else {
                std::istringstream buffer(string_input);
                std::string current;
                int count = 0;
                while (std::getline(buffer, current, '/')) {
                    // 0
                    int index = std::stoi(current) - 1;
                    if (count == 0) {
                        combined_vertices.push_back(vertices.at((index*3)));
                        combined_vertices.push_back(vertices.at((index*3) + 1));
                        combined_vertices.push_back(vertices.at((index*3) + 2));
                        count ++;
                    }
                    // 1
                    else if (count == 1) {
                        combined_vertices.push_back(texture_coords.at(index*2));
                        combined_vertices.push_back(texture_coords.at((index*2) + 1));
                        count ++;
                    }
                    // 2
                    else if (count == 2) {
                        combined_vertices.push_back(normals.at((index*3)));
                        combined_vertices.push_back(normals.at((index*3) + 1));
                        combined_vertices.push_back(normals.at((index*3) + 2));
                        count ++;
                    }

                int mapped_index = 0;
                int mod_is_zero = (global_count - 3) % 6; 
                if (mod_is_zero == 0) {
                    factor += 2;
                    mapped_index = global_count - factor - 1;
                } else {
                    mapped_index = global_count - factor;
                }
                indices.push_back(mapped_index);
                global_count++;
                
            }
                count = 0;
            }
                spdlog::info("String does not have newline: {}", string_input);
            }
        }
    catch (const std::exception &e) {
        spdlog::warn("invalid: {}",e.what());
    }
}
        
GLuint Mesh::getVertexArrayID() const {
    return vertex_array_id;
}

void Mesh::setup_buffers(const std::string &f_name) {


    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);

    int global_count = 3;
    int factor = 0;

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
                add_to_a_buffer(value,last_type, global_count,factor);
                last_type = t;
            } catch (const std::exception &e) {
                spdlog::warn("out of bounds, {}",e.what());
            }

        } else {
            /* NOT A NEW LINE*/
            add_to_a_buffer(line_i,last_type, global_count, factor);
       }
    }


}

void Mesh::create_gl_buffers() {
    glGenBuffers(1,&vertex_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER,vertex_buffer_id);
    glBufferData(GL_ARRAY_BUFFER,combined_vertices.size()*sizeof(float),combined_vertices.data(),GL_STATIC_DRAW);

    glGenVertexArrays(1,&vertex_array_id);
    glBindVertexArray(vertex_array_id);

    glBindBuffer(GL_ARRAY_BUFFER,vertex_buffer_id);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3*sizeof(float)));
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(5*sizeof(float)));
   
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