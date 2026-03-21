
#include "../include/vertex.hpp"

// REVIEW: trailing semicolon after the function body is unnecessary (harmless but
// non-idiomatic). This constructor is simple enough to be defined inline in the header,
// eliminating the need for vertex.cpp entirely.
Vertex::Vertex(float xi, float yi, float zi, float ui, float vi, float wi) : x{xi}, y{yi}, z{zi}, u{ui}, v{vi}, w{wi} {};