
#pragma once

// REVIEW: "GLAD/glad.h" uses uppercase GLAD but the directory is likely "glad/glad.h".
// This works on Windows but will break on case-sensitive systems.
#include <GLAD/glad.h>
// REVIEW: mesh.hpp includes itself (circular include). #pragma once prevents infinite
// recursion, but this is pointless — remove this self-include.
#include "../include/mesh.hpp"
#include <vector>
#include <string>

class Mesh {
    public:
    void drawMesh(GLuint program);
    // REVIEW: should take const std::string& to avoid a copy. Also, marking this
    // explicit would prevent accidental implicit conversions from string to Mesh.
    Mesh(std::string obj_file);
    GLuint getVertexArrayID() const;
    // REVIEW: indices is public while all other data members are private. Either make it
    // private with a getter, or document why it needs external access (Object::draw uses it).
    std::vector<GLuint> indices;

    private:
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texture_coords;
    std::vector<float> combined_vertices;

    GLuint vertex_buffer_id;
    GLuint vertex_array_id;


    void add_to_a_buffer(const std::string &string_input, const std::string &last_type, int& global_count, int& factor); 
    void create_gl_buffers();
    void setup_buffers(const std::string &f_name);

};