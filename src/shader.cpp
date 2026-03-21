
#include "../include/glad/glad.h"

#include "../include/shader.hpp"
#include <spdlog/spdlog.h>
#include <fstream>
#include <sstream>

// REVIEW: vertex_path and fragment_path are passed by value — use const std::string&
// to avoid unnecessary copies. Same applies to setMat4, setVec3, setInt below.
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

// REVIEW: no link status check after glLinkProgram. If linking fails (e.g. mismatched
// varyings between vertex and fragment shaders), it will silently produce a broken program.
// Call glGetProgramiv(program_id, GL_LINK_STATUS, &status) and log any errors.
void Shader::create_program(GLuint vertex_id, GLuint fragment_id) {
    program_id = glCreateProgram();
    glAttachShader(program_id, vertex_id);
    glAttachShader(program_id, fragment_id);
    glLinkProgram(program_id);
}

void Shader::compile_shader(std::string shader_path, SHADER_TYPE type) {
    std::ifstream file(shader_path);
    if (!file.is_open()) {
        // REVIEW(BUG): spdlog uses {} format specifiers, not comma-separated args.
        // This logs "Failed to open file at path: " without printing shader_path.
        // Fix: spdlog::warn("Failed to open file at path: {}", shader_path);
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
    // REVIEW: check_compile_status exists but is never called here. Compile errors are
    // silently ignored. Furthermore, check_compile_status itself reads the status but
    // never logs or returns it — it needs to actually do something with the result.
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

// REVIEW(BUG): this function queries the compile/link status but does nothing with it.
// 'parameter' is never checked or returned. It should:
//   1. Check if parameter == GL_FALSE
//   2. Call glGetShaderInfoLog / glGetProgramInfoLog to retrieve the error message
//   3. Log the error
// Also, the PROGRAM case uses GL_COMPILE_STATUS but should use GL_LINK_STATUS.
// And the spdlog message "Default case ran." is misleading — it's not a default case.
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

// REVIEW: glGetUniformLocation is called every time setMat4/setVec3/setInt are called.
// This is a relatively expensive call. Consider caching uniform locations in a
// std::unordered_map<std::string, GLint> after the program is linked.
// Also, glUseProgram(program_id) / glUseProgram(0) on every uniform set is wasteful —
// batch uniform updates while the program is already bound.
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
