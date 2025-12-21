
#pragma once

#include <GLAD/glad.h>
#include "../include/mesh.hpp"
#include <vector>
#include <string>

class Mesh {
    public:
    void drawMesh(GLuint program);
    Mesh(std::string obj_file);

    private:
    std::vector<float> vertices;
    std::vector<GLuint> indices;
    std::vector<float> normals;
    std::vector<float> texture_coords;
    std::vector<float> combined_vertices;

    GLuint vertex_buffer_id;
    GLuint vertex_array_id;


    void add_to_a_buffer(const std::string &string_input, const std::string &last_type, int& global_count, int& factor); 
    void create_gl_buffers();
    void setup_buffers(const std::string &f_name);

};