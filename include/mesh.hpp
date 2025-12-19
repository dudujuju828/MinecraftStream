
#pragma once

#include "../include/mesh.hpp"
#include <vector>
#include <string>

class Mesh {
    public:
    void drawMesh(int program);
    Mesh(std::string obj_file);

    private:
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texture_coords;

    GLuint vertex_buffer_id;
    GLuint normal_buffer_id;
    GLuint texture_buffer_id;
    GLuint vertex_array_id;


    void add_to_a_buffer(const std::string &string_input, const std::string &last_type); 
    void create_gl_buffers();
    void setup_buffers(const std::string &f_name);

};