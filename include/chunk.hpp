
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <string_view>
#include <string>
#include <unordered_map>
#include "../include/math/vector_math.hpp"


enum class cube_face {
    LEFT,
    RIGHT,
    FRONT,
    BACK,
    TOP,
    BOTTOM
};

enum class BLOCK_TYPE {
    AIR,
    DIRT,
    ICE,
    STONE,
    REDSTONE
};

struct Vertex {
    Vertex(float xi, float yi, float zi, float ui, float vi, float wi) : x{xi}, y{yi}, z{zi}, u{ui}, v{vi}, w{wi} {}
    float x, y, z, u, v, w;
};

class Chunk {
    public:
    Chunk(std::string_view file_name, vecmath::Vector3 position);
    Chunk(std::string_view file_name, int x, int y, int z);


    void constructMesh();
    void print() const;
    const int SIZE = 16;
    vecmath::Vector3 position;
    void setBlock(BLOCK_TYPE b_type, int x, int z, int y);
    void destroyBlock(int x, int z, int y);
    void reconstruct();
    int getChunkBufferSize() const;
    void draw();

    private:
    int bufferSize;
    int vertexCount;
    bool _is_initialized;
    GLuint VBO;
    GLuint VAO;
    bool _dirty;
    void addFace(std::vector<Vertex> &vertex_vector, const cube_face& face_type, int x_offset, int y_offset, int z_offset, int texture_type);
    std::vector<BLOCK_TYPE> chunk;
    BLOCK_TYPE getBlock(int x, int z, int y) const;
    const std::unordered_map<BLOCK_TYPE,int> block_texture_map {
        {BLOCK_TYPE::REDSTONE, 0},
        {BLOCK_TYPE::ICE,1},
        {BLOCK_TYPE::DIRT,2},
        {BLOCK_TYPE::STONE,3}

    };
};
