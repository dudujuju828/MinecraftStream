
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
        // REVIEW: the int constructor can just use the initializer list directly:
        //   Vector3(int x_in, int y_in, int z_in) : x{static_cast<float>(x_in)}, y{...}, z{...} {}
        // No need for the body assignments. static_cast is preferred over C-style (float) casts.
        Vector3(int x_in, int y_in, int z_in) : x{0.0f}, y{0.0f}, z{0.0f} {
            // can this be done better
            x = (float)x_in;
            y = (float)y_in;
            z = (float)z_in;
        }
        // REVIEW(BUG): copy constructor takes a non-const reference. This prevents copying
        // from temporaries/rvalues (e.g. Vector3 v = someFunction();). Should be:
        //   Vector3(const Vector3 &vec_in)
        Vector3(Vector3 &vec_in) : x{vec_in.x}, y{vec_in.y}, z{vec_in.z} {}
        Vector3() : x{0.0f}, y{0.0f}, z{0.0f} {}
        float x,y,z;
        // REVIEW: length() should be marked const — it doesn't modify the vector.
        float length() {
            float squared_sum = x*x + y*y + z*z;
            return std::sqrt(squared_sum);
        }
        // REVIEW: normalize() mutates in-place, which is fine, but consider also providing
        // a const normalized() that returns a new vector. The assert will crash in release
        // builds if stripped — consider a runtime check or epsilon comparison instead of
        // exact float == 0 comparison.
        void normalize() {
            float vec_length = length();
            assert(vec_length != 0);
            float one_over_vec_length = 1/vec_length;
            x *= one_over_vec_length;
            y *= one_over_vec_length;
            z *= one_over_vec_length;
        }

        // REVIEW: cross() should be marked const — it doesn't modify this vector.
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

        // REVIEW(BUG): operator* should NOT mutate 'this' — it's supposed to return a new
        // vector. Currently it modifies the left-hand side in-place AND returns a reference
        // to it. This causes subtle bugs in camera.cpp where "front * speed" permanently
        // mutates the front vector. Fix: return a new Vector3 by value:
        //   Vector3 operator*(float scale) const { return Vector3(x*scale, y*scale, z*scale); }
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
        // REVIEW: unary operator-() should be const — it returns a new vector.
        Vector3 operator-() {
            float x_copy = this->x;
            float y_copy = this->y;
            float z_copy = this->z;
            x_copy *= -1.0f;
            y_copy *= -1.0f;
            z_copy *= -1.0f;
            return Vector3(x_copy,y_copy,z_copy);
        } 

        // REVIEW: print() should be const.
        void print() {
            spdlog::info("VecX: {}\nVecY: {}\nVecZ: {}",x,y,z);
        }
    };

    struct Vector4 {
        Vector4(float x_in, float y_in, float z_in, float w_in) : x{x_in}, y{y_in}, z{z_in}, w{w_in} {}
        // REVIEW(BUG): same as Vector3 — copy constructors take non-const references.
        // Should be (const Vector4&) and (const Vector3&) respectively.
        Vector4(Vector4 &vec_in) : x{vec_in.x}, y{vec_in.y}, z{vec_in.z}, w{vec_in.w} {}
        Vector4(Vector3 &vec_in, float w_in) : x{vec_in.x}, y{vec_in.y}, z{vec_in.z}, w{w_in} {}
        Vector4() : x{0.0f}, y{0.0f}, z{0.0f}, w{0.0f} {}
        float x,y,z,w;

        // REVIEW(BUG): dot product ignores the w component. For a true 4D dot product
        // this should include + this->w*other.w. If this is intentional (treating these
        // as 3D homogeneous coordinates), add a comment explaining why w is excluded.
        float dot(const Vector4& other) const {
            return this->x*other.x + this->y*other.y + this->z*other.z;
        }
    };

   class Matrix44 {
        public:
        // REVIEW: consider using std::array<float,16> instead of std::vector<float>.
        // The matrix is always exactly 16 floats — a fixed-size array avoids a heap
        // allocation per matrix and is more cache-friendly.
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
        // REVIEW(BUG): `const int copy = *(buf+i)` truncates each float to an integer
        // before storing it. A value like 0.5f becomes 0. Should be:
        //   const float copy = *(buf+i);
        // Also, no bounds check that `size <= 16` — passing size > 16 will throw.
        Matrix44(float* buf, const int size) : buffer(16,0.0f) {
            for (int i = 0; i < size; i++) {
                const int copy = *(buf+i);
                buffer.at(i) = copy;
            }
        }

        // REVIEW(BUG): this operator reads from and writes to the same buffer mid-loop.
        // After writing buffer[i + j*4] in an early iteration, later iterations that read
        // the same row via get_row(i) will pick up the already-overwritten values instead
        // of the original ones. Fix: compute results into a temporary buffer first, then
        // copy it back:
        //   std::vector<float> result(16, 0.0f);
        //   ... result[i + j*4] = i_j_value; ...
        //   buffer = result;
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

        // REVIEW: this scales all 16 elements except [15], but that also scales the
        // translation column (indices 3, 7, 11) and the perspective row (index 12, 13, 14).
        // A proper scale matrix should only multiply the diagonal elements [0], [5], [10].
        // If the intent is to scale vertex positions, multiply by a scale matrix instead.
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
        // REVIEW: this is overcomplicated. Just return buffer.data() — same result, one line.
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

        // REVIEW: get_row should be const (like get_col is). Without const, it can't be
        // called on const Matrix44 references.
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