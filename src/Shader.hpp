#pragma once
#include <string>
#include <glad/gl.h>

namespace warp {

class Shader
{
public:
    explicit Shader();
    ~Shader();

    void use() const;
    void clear();
    void from_file(std::string_view vertex_path, std::string_view fragment_path);
    void from_string(std::string_view vertex_str, std::string_view fragment_str);
    [[nodiscard]] GLuint get_projection_location() const;
    [[nodiscard]] GLuint get_model_location() const;

private:
    void compile(std::string_view vertex_str, std::string_view fragment_str);
    void add(std::string_view shader_str, GLenum type);

private:
    GLuint _program;
    GLuint _uniform_projection;
    GLuint _uniform_model;

    // error handling
    GLint _result;
    GLchar _error_log[1024];
};

} // warp
