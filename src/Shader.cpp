#include <iostream>
#include <cassert>
#include <fstream>
#include <sstream>

#include "Shader.hpp"

namespace warp {
Shader::Shader()
        : _program(0),
          _uniform_projection(0),
          _uniform_model(0),
          _result(0),
          _error_log{}
{}

Shader::~Shader()
{
    clear();
}

void Shader::use() const
{
    assert(_program != 0);
    glUseProgram(_program);
}

void Shader::clear()
{
    if (_program) {
        glDeleteProgram(_program);
        _program = 0;
    }
    _uniform_model = 0;
    _uniform_projection = 0;
}

void Shader::from_file(std::string_view vertex_path, std::string_view fragment_path)
{
    std::ifstream vertex_file(vertex_path.data(), std::ios::in);
    std::ifstream fragment_file(fragment_path.data(), std::ios::in);

    std::stringstream vertex_ss = std::stringstream() << (vertex_file.rdbuf());
    std::stringstream fragment_ss = std::stringstream() << fragment_file.rdbuf();

    from_string(vertex_ss.view(), fragment_ss.view());
}

void Shader::from_string(std::string_view vertex_str, std::string_view fragment_str)
{
    assert(!vertex_str.empty() && !fragment_str.empty());
    compile(vertex_str, fragment_str);
}

GLuint Shader::get_projection_location() const
{
    return _uniform_projection;
}

GLuint Shader::get_model_location() const
{
    return _uniform_model;
}

void Shader::compile(std::string_view vertex_str, std::string_view fragment_str)
{
    assert(_program == 0);
    _program = glCreateProgram();

    [[unlikely]] if (!_program) {
        std::cout << "Error creating shader program" << std::endl;
        return;
    }

    add(vertex_str, GL_VERTEX_SHADER);
    add(fragment_str, GL_FRAGMENT_SHADER);

    glLinkProgram(_program);
    glGetProgramiv(_program, GL_LINK_STATUS, &_result);
    [[unlikely]] if (!_result) {
        glGetProgramInfoLog(_program, sizeof(_error_log), nullptr, _error_log);
        std::cout << "Error linking program: " << _error_log << std::endl;
        return;
    }

    glValidateProgram(_program);
    glGetProgramiv(_program, GL_VALIDATE_STATUS, &_result);
    [[unlikely]] if (!_result) {
        glGetProgramInfoLog(_program, sizeof(_error_log), nullptr, _error_log);
        std::cout << "Error while validating program: " << _error_log << std::endl;
        return;
    }

    _uniform_projection = glGetUniformLocation(_program, "projection");
    _uniform_model = glGetUniformLocation(_program, "model");
}

void Shader::add(std::string_view shader_str, GLenum type)
{
    GLuint shader = glCreateShader(type);

    const GLchar *data = shader_str.data();
    const auto len = static_cast<GLsizei>(shader_str.length());

    glShaderSource(shader, 1, &data, &len);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &_result);
    [[unlikely]] if (!_result) {
        glGetShaderInfoLog(shader, sizeof(_error_log), nullptr, _error_log);
        std::cout << "Error compiling shader: " << _error_log << std::endl;
        return;
    }

    glAttachShader(_program, shader);
}

} // warp