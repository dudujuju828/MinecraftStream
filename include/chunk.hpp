
#pragma once
#include <vector>
#include <string_view>
#include <string>

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
    DIRT
};

struct Vertex {
    Vertex(float xi, float yi, float zi, float ui, float vi, float wi) : x{xi}, y{yi}, z{zi}, u{ui}, v{vi}, w{wi} {}
    float x, y, z, u, v, w;
};

class Chunk {
    public:
    const int SIZE = 16;
    Chunk(std::string_view file_name);
    void print() const;
    std::vector<Vertex> constructMesh();
    private:
    void addFace(std::vector<Vertex> &vertex_vector, const cube_face& face_type, int x_offset, int y_offset, int z_offset, int texture_type);
    std::vector<BLOCK_TYPE> chunk;
    BLOCK_TYPE getBlock(int x, int z, int y);
};
