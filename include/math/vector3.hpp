#pragma once

struct Vector3 {
    Vector3(float x_in, float y_in, float z_in) : x{x_in}, y{y_in}, z{z_in} {}
    Vector3(Vector3 &vec_in) : x{vec_in.x}, y{vec_in.y}, z{vec_in.z} {}
    Vector3() : x{0.0f}, y{0.0f}, z{0.0f} {}
    float x,y,z;
};