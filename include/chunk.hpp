
#pragma once
#include <vector>
#include <string_view>
#include <string>

enum class BLOCK_TYPE {
    AIR,
    DIRT
};

class Chunk {
    public:
    const int SIZE = 16;
    Chunk(std::string_view file_name);
    void print() const;
    private:
    std::vector<BLOCK_TYPE> chunk;
};
