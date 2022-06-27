#pragma once
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include <glm/glm.hpp>

class CMesh
{
protected:
	CVertexArray m_VertexArray;
	CElementBuffer m_ElementBuffer;
	CVertexBuffer m_VertexBuffer;

	bool m_bUpdateVertexArray;
	virtual void UpdateVertexArray();

public:
	std::vector <CTexture*> m_vTextures;
	CShader* m_pShader;

	CMesh();
	CMesh(std::vector<stVertex>& _vVerticies, std::vector<GLuint>& _vIndicies, std::vector<CTexture*>& _Textures, CShader* _Shader);
	~CMesh() {};

	const std::vector<stVertex> GetVerticies() const;
	void SetVerticies(const std::vector<stVertex> _vVerticies);
	const std::vector<GLuint> GetIndicies() const;
	void SetIndicies(const std::vector<GLuint> _vIndicies);

	void Draw(const CCamera& _Camera);
};