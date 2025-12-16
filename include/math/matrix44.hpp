
#pragma once
#include <vector>
#include <algorithm>
#include <exception>

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
        buffer.at(0) = 4;
        buffer.at(5) = 4;
    }

    Matrix44& scale(const float& scalar) {
        this->print_buf();
        std::for_each(buffer.begin(),buffer.end(),[&scalar](float &i){i *= scalar;});
        this->print_buf();
        return *this;
    }

    void print_buf() {
        std::for_each(buffer.begin(),buffer.end(),[](float i){std::cout << i << ' ';});
    }
    const float* get_buf() const {
        return buffer.data();
    }


};