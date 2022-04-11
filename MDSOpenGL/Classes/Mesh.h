#pragma once

#include <string>
#include "VertexArray.h"
#include "ElementBuffer.h"
#include "Camera.h"
#include "Texture.h"

class CMesh
{
private:
	CVertexArray m_VertexArray;
	std::vector <stVertex> m_vVerticies;
	std::vector <GLuint> m_vIndicies;
	
	bool m_bUpdateVertexArray;
	void UpdateVertexArray();

public:
	std::vector <CTexture> m_vTextures;
	CShader* m_pShader;

	CMesh(std::vector<stVertex>& _vVerticies, std::vector<GLuint>& _vIndicies, std::vector <CTexture>& _Textures, CShader* _pShader);

	std::vector <stVertex> GetVerticies() const;
	void SetVerticies(std::vector<stVertex>& _vVerticies);
	std::vector <GLuint> GetIndicies() const;
	void SetIndicies(std::vector<GLuint>& _vIndicies);

	void Draw(CCamera& _Camera);
};