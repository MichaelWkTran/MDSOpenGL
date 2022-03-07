#include "VertexArray.h"

CVertexArray::CVertexArray()
{
    glGenVertexArrays(1, &m_GLuID);
}

void CVertexArray::LinkAttribute
(
    CVertexBuffer& VertexBuffer,
    GLuint GluLayout,
    GLuint _GLuNumComponents,
    GLenum _GLeType,
    GLsizeiptr _Stride,
    void* _Offset
)
{
    VertexBuffer.Bind();

    glVertexAttribPointer(GluLayout, _GLuNumComponents, _GLeType, GL_FALSE, _Stride, _Offset);
    glEnableVertexAttribArray(GluLayout);

    VertexBuffer.Unbind();
}

void CVertexArray::Bind()
{
    glBindVertexArray(m_GLuID);
}

void CVertexArray::Unbind()
{
    glBindVertexArray(0);
}

void CVertexArray::Delete()
{
    glDeleteVertexArrays(1, &m_GLuID);
}
