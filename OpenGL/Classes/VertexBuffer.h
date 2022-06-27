#pragma once

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

struct stVertex
{
	glm::vec3 m_v3Position;
	glm::vec3 m_v3Normal;
	glm::vec2 m_v2TextureUV;
	glm::vec3 m_v3Colour = glm::vec3(1, 1, 1);
};

class CVertexBuffer
{
private:
	GLuint m_GLuID;
	std::vector <stVertex> m_vVertices;

public:
	CVertexBuffer();
	CVertexBuffer(const std::vector<stVertex> _vVerticies);
	CVertexBuffer(CVertexBuffer const&) = delete;
	~CVertexBuffer();

	const GLuint& GetID();
	const std::vector<stVertex> GetVertices() const;
	void SetVertices(const std::vector<stVertex> _vVertices);
	void Bind();
	void Unbind();
};