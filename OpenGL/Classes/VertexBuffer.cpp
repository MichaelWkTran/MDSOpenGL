#include "VertexBuffer.h"

CVertexBuffer::CVertexBuffer()
{
    glGenBuffers(1, &m_GLuID);
}

CVertexBuffer::CVertexBuffer(const std::vector<stVertex> _vVerticies)
{
    glGenBuffers(1, &m_GLuID);
    SetVertices(_vVerticies);
}

CVertexBuffer::~CVertexBuffer()
{
    glDeleteBuffers(1, &m_GLuID);
}

const GLuint& CVertexBuffer::GetID()
{
    return m_GLuID;
}

const std::vector<stVertex> CVertexBuffer::GetVertices() const
{
    return m_vVertices;
}

void CVertexBuffer::SetVertices(const std::vector<stVertex> _vVertices)
{
    m_vVertices = _vVertices;

    Bind();
    glBufferData(GL_ARRAY_BUFFER, m_vVertices.size() * sizeof(stVertex), m_vVertices.data(), GL_STATIC_DRAW);
    Unbind();
}

void CVertexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_GLuID);
}

void CVertexBuffer::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}