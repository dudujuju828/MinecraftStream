
#pragma once
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
    STONE
};

struct Vertex {
    Vertex(float xi, float yi, float zi, float ui, float vi, float wi) : x{xi}, y{yi}, z{zi}, u{ui}, v{vi}, w{wi} {}
    float x, y, z, u, v, w;
};

class Chunk {
    public:
    Chunk(std::string_view file_name, vecmath::Vector3 position);


    std::vector<Vertex> constructMesh();
    void print() const;
    const int SIZE = 16;
    vecmath::Vector3 position;

    private:
    void addFace(std::vector<Vertex> &vertex_vector, const cube_face& face_type, int x_offset, int y_offset, int z_offset, int texture_type);
    std::vector<BLOCK_TYPE> chunk;
    BLOCK_TYPE getBlock(int x, int z, int y);
    const std::unordered_map<BLOCK_TYPE,int> block_texture_map {
        {BLOCK_TYPE::ICE,0},
        {BLOCK_TYPE::DIRT,1},
        {BLOCK_TYPE::STONE,2}
    };
};
