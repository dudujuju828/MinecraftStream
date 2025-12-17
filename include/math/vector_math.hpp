
#pragma once
#include <vector>
#include <algorithm>
#include <iostream>
#include <exception>

namespace vecmath {
    class Matrix44 {
        public:
        std::vector<float> buffer; 

        Matrix44() : buffer(16,0.0f) {}
        /*
        writes from the beginning BEWARNED 
        */
        Matrix44(float* buf, const int size) : buffer(16,0.0f) {
            for (int i = 0; i < size; i++) {
                const int copy = *(buf+i);
                buffer.at(i) = copy;
            }
        }

        Matrix44& scale(const float& scalar) {
            for (int i = 0; i < 16; i++) {
                if (i != 15) {
                    buffer.at(i) *= scalar; 
                }
            }

            return *this;
        }

        void print_buf() {
            std::for_each(buffer.begin(),buffer.end(),[](float i){std::cout << i << ' ';});
        }
        const float* get_buf() const {
            const float& val = buffer[0];
            const float * first_element = &val; 
            return first_element;
        }


    };

    struct Vector3 {
        Vector3(float x_in, float y_in, float z_in) : x{x_in}, y{y_in}, z{z_in} {}
        Vector3(Vector3 &vec_in) : x{vec_in.x}, y{vec_in.y}, z{vec_in.z} {}
        Vector3() : x{0.0f}, y{0.0f}, z{0.0f} {}
        float x,y,z;
    };

    struct Vector4 {
        Vector4(float x_in, float y_in, float z_in, float w_in) : x{x_in}, y{y_in}, z{z_in}, w{w_in} {}
        Vector4(Vector4 &vec_in) : x{vec_in.x}, y{vec_in.y}, z{vec_in.z}, w{vec_in.w} {}
        Vector4() : x{0.0f}, y{0.0f}, z{0.0f}, w{0.0f} {}
        float x,y,z,w;
    };

};