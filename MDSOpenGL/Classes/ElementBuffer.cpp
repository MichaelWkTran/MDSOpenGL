#include "ElementBuffer.h"
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

CElementBuffer::CElementBuffer()
{
    glGenBuffers(1, &m_GLuID);
}

CElementBuffer::CElementBuffer(const std::vector<unsigned int> _GLuIndicies)
{
    glGenBuffers(1, &m_GLuID);
    SetIndicies(_GLuIndicies);
}

CElementBuffer::~CElementBuffer()
{
    glDeleteBuffers(1, &m_GLuID);
}

const unsigned int& CElementBuffer::GetID()
{
    return m_GLuID;
}

const std::vector<unsigned int> CElementBuffer::GetIndicies() const
{
    return m_vIndicies;
}

void CElementBuffer::SetIndicies(const std::vector<unsigned int> _vIndicies)
{
    m_vIndicies = _vIndicies;

    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vIndicies.size() * sizeof(unsigned int), m_vIndicies.data(), GL_STATIC_DRAW);
    Unbind();
}

void CElementBuffer::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_GLuID);
}

void CElementBuffer::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}