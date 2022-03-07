#include "VertexBuffer.h"

CVertexBuffer::CVertexBuffer(std::vector<stVertex>& _stVerticies)
{
	glGenBuffers(1, &m_GLuID);
    glBindBuffer(GL_ARRAY_BUFFER, m_GLuID);
    glBufferData(GL_ARRAY_BUFFER, _stVerticies.size() * sizeof(stVertex), _stVerticies.data(), GL_STATIC_DRAW);
}

void CVertexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_GLuID);
}

void CVertexBuffer::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CVertexBuffer::Delete()
{
    glDeleteBuffers(1, &m_GLuID);
}
