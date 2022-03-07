#pragma once

#include <string>
#include "VertexArray.h"
#include "ElementBuffer.h"
#include "Camera.h"
#include "Texture.h"

class CMesh
{
public:
	std::vector <stVertex> m_vVerticies;
	std::vector <GLuint> m_vIndicies;
	std::vector <CTexture> m_vTextures;
	CVertexArray m_VertexArray;

	CMesh(std::vector<stVertex>& _vVerticies, std::vector<GLuint>& _GLuIndicies, std::vector <CTexture>& _Textures);

	void Draw(CShader& _Shader, CCamera& _Camera);
};