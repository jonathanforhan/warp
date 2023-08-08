#include "Mesh.hpp"
#include <cassert>

namespace warp {

Mesh::Mesh()
        : _vao(0),
          _vbo(0),
          _ebo(0),
          _index_count(0)
{}

Mesh::~Mesh()
{
    clear();
}

void Mesh::create(const std::vector<GLfloat> &vertices, const std::vector<GLuint> &indices)
{
    assert(vertices.size() > 1 && indices.size() > 1);

    _index_count = static_cast<GLsizei>(indices.size());

    glCreateVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    glCreateBuffers(1, &_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 GLsizeiptr(sizeof(GLuint) * indices.size()),
                 indices.data(),
                 GL_STATIC_DRAW);

    glCreateBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 GLsizeiptr(sizeof(GLfloat) * vertices.size()),
                 vertices.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::render() const
{
    glBindVertexArray(_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glDrawElements(GL_TRIANGLES, _index_count, GL_UNSIGNED_INT, nullptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::clear()
{
    if (_ebo) {
        glDeleteBuffers(1, &_ebo);
        _ebo = 0;
    }
    if (_vbo) {
        glDeleteBuffers(1, &_vbo);
        _vbo = 0;
    }
    if (_vao) {
        glDeleteVertexArrays(1, &_vao);
        _vao = 0;
    }
    _index_count = 0;
}

} // warp