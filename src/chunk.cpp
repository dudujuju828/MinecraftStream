
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../include/chunk.hpp"
#include "../include/math/vector_math.hpp"
#include <iostream>
#include <string_view>
#include <fstream>
#include <exception>
#include <spdlog/spdlog.h>
#include <algorithm>


Chunk::Chunk(std::string_view filename, vecmath::Vector3 pos) : position{pos}, _is_initialized{false}, _dirty{true} {
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
                case 2:
                chunk.push_back(BLOCK_TYPE::ICE);
                break;
                case 3:
                chunk.push_back(BLOCK_TYPE::STONE);
                default:
                chunk.push_back(BLOCK_TYPE::DIRT); 
                break;
            }
        } catch (const std::exception& e) {
            spdlog::error("Error loading chunkfile :{}\nException: {}", filename.data(),e.what());
        }
    }
}

void Chunk::draw() {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void Chunk::reconstruct() {

    if (_dirty) {
        constructMesh();
        //final step is to reupload data (done in constructMesh)
    }
    _dirty = false;

}

void Chunk::constructMesh() {

    if (!_is_initialized) {
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, 216 * SIZE * SIZE * SIZE * sizeof(float), NULL, GL_STREAM_DRAW);
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);
        _is_initialized = true;
    }

    std::vector<Vertex> v;
    for (int y = 0; y < SIZE; y++) {
        for (int z = 0; z < SIZE; z++) {
            for (int x = 0; x < SIZE; x++) {
                /* check the neighbouring 6 blocks */
                /* assume air */
                BLOCK_TYPE current_block = getBlock(x,z,y);
                if (current_block == BLOCK_TYPE::AIR) continue;
                BLOCK_TYPE top_block = BLOCK_TYPE::AIR;
                BLOCK_TYPE bottom_block = BLOCK_TYPE::AIR;
                BLOCK_TYPE left_block = BLOCK_TYPE::AIR;
                BLOCK_TYPE right_block = BLOCK_TYPE::AIR;
                BLOCK_TYPE front_block = BLOCK_TYPE::AIR;
                BLOCK_TYPE back_block = BLOCK_TYPE::AIR;


                /* I think the IF conditions are all wrong here*/
                /* continue on the edge cases LIKELY WRONG, but avoids index crashing*/
                if (x <= 0) {
                    //addFace(v, cube_face::LEFT, x,z,y, 1);
                } else {
                    if (x-1 < SIZE) left_block = getBlock(x-1,z,y);
                }
                if (x + 1 >= SIZE) {
                    //addFace(v, cube_face::RIGHT, x,z,y, 1);
                } else {
                    right_block = getBlock(x+1, z, y);
                }
                if (y <= 0) {
                    //addFace(v, cube_face::TOP, x,z,y, 1);
                } else {
                    if (y - 1 < SIZE) bottom_block = getBlock(x, z, y-1);
                }
                if (y + 1 >= SIZE) {
                    //addFace(v, cube_face::BOTTOM, x,z,y, 1);
                } else {
                    top_block = getBlock(x, z, y+1);
                }
                if (z <= 0) {
                    //addFace(v, cube_face::FRONT, x,z,y, 1);
                } else {
                    if (z-1 < SIZE) front_block = getBlock(x, z-1, y);
                }
                if (z + 1 >= SIZE) {
                    //addFace(v, cube_face::BACK, x,z,y, 0);
                } else {
                    back_block = getBlock(x, z+1, y);
                }
                auto it = block_texture_map.find(current_block);
                int block_type;
                if (it != block_texture_map.end()) {

                    block_type = it->second;
                } 

                if (top_block == BLOCK_TYPE::AIR) {
                    addFace(v, cube_face::TOP, x, z, y, block_type);
                }
                if (bottom_block == BLOCK_TYPE::AIR) {
                    addFace(v, cube_face::BOTTOM, x, z, y, block_type);
                }
                if (left_block == BLOCK_TYPE::AIR) {
                    addFace(v, cube_face::LEFT, x, z, y,  block_type);
                }
                if (right_block == BLOCK_TYPE::AIR) {
                    addFace(v, cube_face::RIGHT, x, z, y, block_type);
                }
                if (front_block == BLOCK_TYPE::AIR) {
                    addFace(v, cube_face::FRONT, x, z, y, block_type);
                }
                if (back_block == BLOCK_TYPE::AIR) {
                    addFace(v, cube_face::BACK, x, z, y,  block_type);
                }
                
            }
        }
    }

    bufferSize = v.size() * 6 * sizeof(float);
    vertexCount = v.size();

    if (!_is_initialized) {
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, bufferSize, NULL, GL_STREAM_DRAW);
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);
        _is_initialized = true;
    }

    std::vector<float> flattened_data;
    if (_dirty)  {
        for (auto &vertex : v) {
            /* potential problem with xyz xzy*/
            flattened_data.push_back(vertex.x);
            flattened_data.push_back(vertex.y);
            flattened_data.push_back(vertex.z);
            flattened_data.push_back(vertex.u);
            flattened_data.push_back(vertex.v);
            flattened_data.push_back(vertex.w);
        }
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, flattened_data.size() * sizeof(float), flattened_data.data(), GL_STATIC_DRAW);
    }

}

BLOCK_TYPE Chunk::getBlock(int x, int z, int y) const {
    return chunk.at(x + (16*z) + (16*16*y));
}

void Chunk::addFace(std::vector<Vertex> &vertex_vector, const cube_face& face_type, int x_offset, int z_offset, int y_offset, int texture_type) {
    x_offset += position.x * SIZE;
    y_offset += position.y * SIZE;
    z_offset += position.z * SIZE;
    switch(face_type) {
        case cube_face::FRONT:
            vertex_vector.push_back(Vertex(0.0f + x_offset, 0.0f + y_offset, 0.0f + z_offset, 0.0f, 0.0f, texture_type));
            vertex_vector.push_back(Vertex(1.0f + x_offset, 0.0f + y_offset, 0.0f + z_offset, 1.0f, 0.0f, texture_type));
            vertex_vector.push_back(Vertex(0.0f + x_offset, 1.0f + y_offset, 0.0f + z_offset, 0.0f, 1.0f, texture_type));

            vertex_vector.push_back(Vertex(1.0f + x_offset, 1.0f + y_offset, 0.0f + z_offset, 1.0f, 1.0f, texture_type));
            vertex_vector.push_back(Vertex(1.0f + x_offset, 0.0f + y_offset, 0.0f + z_offset, 1.0f, 0.0f, texture_type));
            vertex_vector.push_back(Vertex(0.0f + x_offset, 1.0f + y_offset, 0.0f + z_offset, 0.0f, 1.0f, texture_type));
            break;
        case cube_face::BACK:
            vertex_vector.push_back(Vertex(0.0f + x_offset, 0.0f + y_offset, 1.0f + z_offset, 0.0f, 0.0f, texture_type));
            vertex_vector.push_back(Vertex(1.0f + x_offset, 0.0f + y_offset, 1.0f + z_offset, 1.0f, 0.0f, texture_type));
            vertex_vector.push_back(Vertex(0.0f + x_offset, 1.0f + y_offset, 1.0f + z_offset, 0.0f, 1.0f, texture_type));

            vertex_vector.push_back(Vertex(1.0f + x_offset, 1.0f + y_offset, 1.0f + z_offset, 1.0f, 1.0f, texture_type));
            vertex_vector.push_back(Vertex(1.0f + x_offset, 0.0f + y_offset, 1.0f + z_offset, 1.0f, 0.0f, texture_type));
            vertex_vector.push_back(Vertex(0.0f + x_offset, 1.0f + y_offset, 1.0f + z_offset, 0.0f, 1.0f, texture_type));
            break;
        case cube_face::LEFT:
            // 0,0,0  0,1,0   0,1,1   0,0,1
            vertex_vector.push_back(Vertex(0.0f + x_offset, 0.0f + y_offset, 0.0f + z_offset, 0.0f, 0.0f, texture_type));
            vertex_vector.push_back(Vertex(0.0f + x_offset, 1.0f + y_offset, 0.0f + z_offset, 1.0f, 0.0f, texture_type));
            vertex_vector.push_back(Vertex(0.0f + x_offset, 1.0f + y_offset, 1.0f + z_offset, 0.0f, 1.0f, texture_type));

            vertex_vector.push_back(Vertex(0.0f + x_offset, 0.0f + y_offset, 0.0f + z_offset, 1.0f, 1.0f, texture_type));
            vertex_vector.push_back(Vertex(0.0f + x_offset, 1.0f + y_offset, 1.0f + z_offset, 1.0f, 0.0f, texture_type));
            vertex_vector.push_back(Vertex(0.0f + x_offset, 0.0f + y_offset, 1.0f + z_offset, 0.0f, 1.0f, texture_type));
            break;
        case cube_face::RIGHT:
            // 0,0,0  0,1,0   0,1,1   0,0,1
            vertex_vector.push_back(Vertex(1.0f + x_offset, 0.0f + y_offset, 0.0f + z_offset, 0.0f, 0.0f, texture_type));
            vertex_vector.push_back(Vertex(1.0f + x_offset, 1.0f + y_offset, 0.0f + z_offset, 1.0f, 0.0f, texture_type));
            vertex_vector.push_back(Vertex(1.0f + x_offset, 1.0f + y_offset, 1.0f + z_offset, 0.0f, 1.0f, texture_type));

            vertex_vector.push_back(Vertex(1.0f + x_offset, 0.0f + y_offset, 0.0f + z_offset, 1.0f, 1.0f, texture_type));
            vertex_vector.push_back(Vertex(1.0f + x_offset, 1.0f + y_offset, 1.0f + z_offset, 1.0f, 0.0f, texture_type));
            vertex_vector.push_back(Vertex(1.0f + x_offset, 0.0f + y_offset, 1.0f + z_offset, 0.0f, 1.0f, texture_type));
            break;
        case cube_face::TOP:
            // 0,1,0  1,1,0   0,1,1   1,1,0   1,1,1   0,1,1
            vertex_vector.push_back(Vertex(0.0f + x_offset, 1.0f + y_offset, 0.0f + z_offset, 0.0f, 0.0f, texture_type));
            vertex_vector.push_back(Vertex(1.0f + x_offset, 1.0f + y_offset, 0.0f + z_offset, 1.0f, 0.0f, texture_type));
            vertex_vector.push_back(Vertex(0.0f + x_offset, 1.0f + y_offset, 1.0f + z_offset, 0.0f, 1.0f, texture_type));

            vertex_vector.push_back(Vertex(1.0f + x_offset, 1.0f + y_offset, 0.0f + z_offset, 1.0f, 1.0f, texture_type));
            vertex_vector.push_back(Vertex(1.0f + x_offset, 1.0f + y_offset, 1.0f + z_offset, 1.0f, 0.0f, texture_type));
            vertex_vector.push_back(Vertex(0.0f + x_offset, 1.0f + y_offset, 1.0f + z_offset, 0.0f, 1.0f, texture_type));
            break;
        case cube_face::BOTTOM:
            // 0,1,0  1,1,0   0,1,1   1,1,0   1,1,1   0,1,1
            vertex_vector.push_back(Vertex(0.0f + x_offset, 0.0f + y_offset, 0.0f + z_offset, 0.0f, 0.0f, texture_type));
            vertex_vector.push_back(Vertex(1.0f + x_offset, 0.0f + y_offset, 0.0f + z_offset, 1.0f, 0.0f, texture_type));
            vertex_vector.push_back(Vertex(0.0f + x_offset, 0.0f + y_offset, 1.0f + z_offset, 0.0f, 1.0f, texture_type));

            vertex_vector.push_back(Vertex(1.0f + x_offset, 0.0f + y_offset, 0.0f + z_offset, 1.0f, 1.0f, texture_type));
            vertex_vector.push_back(Vertex(1.0f + x_offset, 0.0f + y_offset, 1.0f + z_offset, 1.0f, 0.0f, texture_type));
            vertex_vector.push_back(Vertex(0.0f + x_offset, 0.0f + y_offset, 1.0f + z_offset, 0.0f, 1.0f, texture_type));
            break;
    }
}

void Chunk::setBlock(BLOCK_TYPE b_type, int x, int y, int z) {
    _dirty = true;
    chunk.at(x + (16*z) + (16*16*y)) = b_type;
}

void Chunk::destroyBlock(int x, int y, int z) {
    setBlock(BLOCK_TYPE::AIR, x, y, z);
}
int Chunk::getChunkBufferSize() const {
    return bufferSize;
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