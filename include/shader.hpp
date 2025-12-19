#pragma once

#include <glad/glad.h>
#include <string>

class Shader {
    private:
    GLuint vertex_id;
    GLuint fragment_id;
    GLuint program_id;

    enum class SHADER_TYPE {
        VERTEX_SHADER,
        FRAGMENT_SHADER,
        PROGRAM
    };

    void compile_shader(std::string shader_path, SHADER_TYPE type);
    void check_compile_status(GLuint shader_object, SHADER_TYPE type);
    void create_program(GLuint vertex_id, GLuint fragment_id);
    public:
    void use_program() const;
    GLint get_program_id() const;
    Shader(std::string vertex_path, std::string fragment_path);
};