#pragma once

#include <glm/glm.hpp>
#include <GLEW/glew.h>
#include <vector>

struct stVertex
{
	glm::vec3 m_v3Position;
	glm::vec3 m_v3Normal;
	glm::vec3 m_v3Colour;
	glm::vec2 m_v3TextureUV;
};

class CVertexBuffer
{
public:
	GLuint m_GLuID;
	CVertexBuffer(std::vector<stVertex>& _stVerticies);

	void Bind();
	void Unbind();
	void Delete();
};