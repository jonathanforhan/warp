#pragma once
#include <vector>
#include <glad/gl.h>

namespace warp {

class Mesh
{
public:
    explicit Mesh();
    ~Mesh();

    void create(const std::vector<GLfloat> &vertices, const std::vector<GLuint> &indices);
    void render() const;
    void clear();

private:
    GLuint _vao;
    GLuint _vbo;
    GLuint _ebo;
    GLsizei _index_count;
};

} // warp
