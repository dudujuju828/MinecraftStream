
#include "../include/chunk.hpp"
#include <iostream>
#include <string_view>
#include <fstream>
#include <exception>
#include <spdlog/spdlog.h>
#include <algorithm>

Chunk::Chunk(std::string_view filename) {
    std::ifstream file(filename.data());
    std::string file_line;
    while(std::getline(file, file_line, ',')) {
        try {
            int type = std::stoi(file_line);
            switch (type) {
                case 0:
                chunk.push_back(BLOCK_TYPE::AIR); 
                break;
                case 1:
                chunk.push_back(BLOCK_TYPE::DIRT); 
                break;
                default:
                chunk.push_back(BLOCK_TYPE::DIRT); 
                break;
            }
        } catch (const std::exception& e) {
            spdlog::error("Error loading chunkfile :{}\nException: {}", filename.data(),e.what());
        }
    }
}

void Chunk::print() const {
    int count = 0;
    std::for_each(chunk.begin(), chunk.end(), [&count](BLOCK_TYPE i){
        switch (i) {
        case BLOCK_TYPE::AIR:
        std::cout << "AIR ";
        break;
        case BLOCK_TYPE::DIRT:
        std::cout << "DRT ";
        break;
        default:
        std::cout << "DRT ";
        break;
    };
    count ++;

    if (count % 16 == 0) {
        std::cout << '\n';
    }

    if (count % 256 == 0) {
        std::cout << '\n';
    }
    
});
}