
#pragma once

// imported into chunk.hpp, and crosshair.hpp
// REVIEW: struct members are already public by default — the 'public:' is redundant.
// Consider adding a comment explaining what u, v, w represent (texture coordinates,
// where w = texture array layer index).
struct Vertex {
    public:
    float x, y, z, u, v, w;
    Vertex(float xi, float yi, float zi, float ui, float vi, float wi);
};
