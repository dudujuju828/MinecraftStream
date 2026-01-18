
#include "../include/glad/glad.h"

#include "../include/shader.hpp"
#include <spdlog/spdlog.h>
#include <fstream>
#include <sstream>

Shader::Shader(std::string vertex_path, std::string fragment_path) {
    spdlog::info("Loading shader.");
    compile_shader(vertex_path, SHADER_TYPE::VERTEX_SHADER);
    compile_shader(fragment_path, SHADER_TYPE::FRAGMENT_SHADER);
    create_program(vertex_id, fragment_id);
    spdlog::info("Loaded shader.");
}

void Shader::use_program() const {
    glUseProgram(program_id);
}

void Shader::create_program(GLuint vertex_id, GLuint fragment_id) {
    program_id = glCreateProgram();
    glAttachShader(program_id, vertex_id);
    glAttachShader(program_id, fragment_id);
    glLinkProgram(program_id);
}

void Shader::compile_shader(std::string shader_path, SHADER_TYPE type) {
    std::ifstream file(shader_path);
    if (!file.is_open()) {
        spdlog::warn("Failed to open file at path: ", shader_path);
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string buffer_str = buffer.str(); 
    const char * c_style_str = buffer_str.c_str();

    GLint shader_object = 0;
    switch (type) {
        case SHADER_TYPE::VERTEX_SHADER:
            shader_object = glCreateShader(GL_VERTEX_SHADER);
            break;
        case SHADER_TYPE::FRAGMENT_SHADER:
            shader_object = glCreateShader(GL_FRAGMENT_SHADER);
            break;
        default:
            spdlog::info("Default case ran.");
            break;
    }
    if (!glIsShader(shader_object)) {
        spdlog::warn("Shader object not created.");
        return;
    } 

    glShaderSource(shader_object,1,&c_style_str,nullptr);
    glCompileShader(shader_object);
    /*CHECK COMPILE STATUS (FINISH SOON)*/
    switch (type) {
        case SHADER_TYPE::VERTEX_SHADER:
            vertex_id = shader_object;
            break;
        case SHADER_TYPE::FRAGMENT_SHADER:
            fragment_id = shader_object;
           break;
        default:
            spdlog::info("Default case ran.");
            break;
    }


}

void Shader::check_compile_status(GLuint object_id, SHADER_TYPE type) {
    GLint parameter;
    switch (type) {
        case SHADER_TYPE::VERTEX_SHADER:
            glGetShaderiv(object_id, GL_COMPILE_STATUS, &parameter);
            break;
        case SHADER_TYPE::FRAGMENT_SHADER:
            glGetShaderiv(object_id, GL_COMPILE_STATUS, &parameter);
            break;
        case SHADER_TYPE::PROGRAM:
            glGetProgramiv(object_id, GL_COMPILE_STATUS, &parameter);
            spdlog::info("Default case ran.");
            break;
    }
}

GLint Shader::get_program_id() const {
    return program_id;
}

void Shader::setMat4(std::string name,vecmath::Matrix44 mat) {
    glUseProgram(program_id);
    glUniformMatrix4fv(glGetUniformLocation(program_id, name.data()), 1, GL_FALSE, mat.get_buf());
    glUseProgram(0);
}

void Shader::setVec3(std::string name, vecmath::Vector3 vec) {
    glUseProgram(program_id);
    glUniform3f(glGetUniformLocation(program_id, name.data()), vec.x, vec.y, vec.z);
    glUseProgram(0);
}

void Shader::setInt(std::string name, int val) {
    glUseProgram(program_id);
    glUniform1i(glGetUniformLocation(program_id, name.data()), val);
    glUseProgram(0);
}
