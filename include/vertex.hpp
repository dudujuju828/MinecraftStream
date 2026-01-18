
#pragma once

// imported into chunk.hpp, and crosshair.hpp
struct Vertex {
    public:
    float x, y, z, u, v, w;
    Vertex(float xi, float yi, float zi, float ui, float vi, float wi);
};
