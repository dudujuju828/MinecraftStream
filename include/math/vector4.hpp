#pragma once

struct Vector4 {
    Vector4(float x_in, float y_in, float z_in, float w_in) : x{x_in}, y{y_in}, z{z_in}, w{w_in} {}
    Vector4(Vector4 &vec_in) : x{vec_in.x}, y{vec_in.y}, z{vec_in.z}, w{vec_in.w} {}
    Vector4() : x{0.0f}, y{0.0f}, z{0.0f}, w{0.0f} {}
    float x,y,z,w;
};