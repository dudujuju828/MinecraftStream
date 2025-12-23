
#pragma once
#include <vector>
#include <algorithm>
#include <iostream>
#include <exception>
#include <cmath>
#include <cassert>
#include <spdlog/spdlog.h>


namespace vecmath {
 
    struct Vector3 {
        Vector3(float x_in, float y_in, float z_in) : x{x_in}, y{y_in}, z{z_in} {}
        Vector3(Vector3 &vec_in) : x{vec_in.x}, y{vec_in.y}, z{vec_in.z} {}
        Vector3() : x{0.0f}, y{0.0f}, z{0.0f} {}
        float x,y,z;
        float length() {
            float squared_sum = x*x + y*y + z*z;
            return std::sqrt(squared_sum);
        }
        void normalize() {
            float vec_length = length();
            assert(vec_length != 0);
            float one_over_vec_length = 1/vec_length;
            x *= one_over_vec_length;
            y *= one_over_vec_length;
            z *= one_over_vec_length;
        }

        Vector3 cross(const Vector3 &other_vec) {
            float x = (this->y * other_vec.z) - (this->z * other_vec.y);
            float y = (this->z * other_vec.x) - (this->x * other_vec.z);
            float z = (this->x * other_vec.y) - (this->y * other_vec.x);
            return Vector3(x,y,z);
        }

        Vector3& operator*=(float scalar) {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }
        Vector3& operator+=(const Vector3 &other_vec) {
            x += other_vec.x;
            y += other_vec.y;
            z += other_vec.z;
            return *this;
        }

        Vector3& operator*(const float scale) {
            x *= scale;
            y *= scale;
            z *= scale;
            return *this;
        }

        Vector3& operator-=(const Vector3 &other_vec) {
            x -= other_vec.x;
            y -= other_vec.y;
            z -= other_vec.z;
            return *this;
        }
        Vector3 operator-() {
            float x_copy = this->x;
            float y_copy = this->y;
            float z_copy = this->z;
            x_copy *= -1.0f;
            y_copy *= -1.0f;
            z_copy *= -1.0f;
            return Vector3(x_copy,y_copy,z_copy);
        } 

        void print() {
            spdlog::info("VecX: {}\nVecY: {}\nVecZ: {}",x,y,z);
        }
    };

    struct Vector4 {
        Vector4(float x_in, float y_in, float z_in, float w_in) : x{x_in}, y{y_in}, z{z_in}, w{w_in} {}
        Vector4(Vector4 &vec_in) : x{vec_in.x}, y{vec_in.y}, z{vec_in.z}, w{vec_in.w} {}
        Vector4(Vector3 &vec_in, float w_in) : x{vec_in.x}, y{vec_in.y}, z{vec_in.z}, w{w_in} {}
        Vector4() : x{0.0f}, y{0.0f}, z{0.0f}, w{0.0f} {}
        float x,y,z,w;

        float dot(const Vector4& other) const {
            return this->x*other.x + this->y*other.y + this->z*other.z;
        }
    };

   class Matrix44 {
        public:
        std::vector<float> buffer; 

        Matrix44() : buffer(16,0.0f) {
            buffer.at(0) = 1.0f;
            buffer.at(5) = 1.0f;
            buffer.at(10) = 1.0f;
            buffer.at(15) = 1.0f;
        }
        /*
        writes from the beginning BEWARNED 
        */
        Matrix44(float* buf, const int size) : buffer(16,0.0f) {
            for (int i = 0; i < size; i++) {
                const int copy = *(buf+i);
                buffer.at(i) = copy;
            }
        }

        Matrix44& operator*=(const Matrix44& other_matrix) {
            for (int i = 0; i < 4; i ++) {
                for (int j = 0; j < 4; j++) {
                    vecmath::Vector4 left_row = this->get_row(i);
                    vecmath::Vector4 right_column = other_matrix.get_col(j);
                    float i_j_value = left_row.dot(right_column);
                    buffer.at(i + j*4) = i_j_value;
                }
            }
            return *this;
        } 

        void scale(const float& scalar) {
            for (int i = 0; i < 16; i++) {
                if (i != 15) {
                    buffer.at(i) *= scalar; 
                }
            }
        }

        void print_buf() {
            std::for_each(buffer.begin(),buffer.end(),[](float i){std::cout << i << ' ';});
        }
        const float* get_buf() const {
            const float& val = buffer[0];
            const float * first_element = &val; 
            return first_element;
        }

        void set_column(vecmath::Vector4 v, int column_index) {
            int starting_index = column_index*4;
            buffer.at(starting_index) = v.x;
            buffer.at(starting_index+1) = v.y;
            buffer.at(starting_index+2) = v.z;
            buffer.at(starting_index+3) = v.w;
        }

        void set_row(vecmath::Vector4 v, int row_index) {
            int starting_index = row_index;
            buffer.at(starting_index) = v.x;
            buffer.at(starting_index+4*1) = v.y;
            buffer.at(starting_index+4*2) = v.z;
            buffer.at(starting_index+4*3) = v.w;

        }

        void set_value(int index, float value) {
            buffer.at(index) = value;
        }

        vecmath::Vector4 get_row(int row_index) {
            int starting_index = row_index;
            float val_x = buffer.at(starting_index);
            float val_y = buffer.at(starting_index+4*1);
            float val_z = buffer.at(starting_index+4*2);
            float val_w = buffer.at(starting_index+4*3);
            return vecmath::Vector4(val_x,val_y,val_z,val_w);
        }

        vecmath::Vector4 get_col(int col_index) const {
            int starting_index = col_index*4;
            float val_x = buffer.at(starting_index);
            float val_y = buffer.at(starting_index+1);
            float val_z = buffer.at(starting_index+2);
            float val_w = buffer.at(starting_index+3);
            return vecmath::Vector4(val_x,val_y,val_z,val_w);
        }

    };

};