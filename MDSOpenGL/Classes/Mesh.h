#pragma once
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include <glm/glm.hpp>

struct stVertex
{
	glm::vec3 v3Position;
	glm::vec3 v3Normal;
	glm::vec2 v2TextureUV;
	glm::vec3 v3Colour = glm::vec3(1, 1, 1);

	static void LinkAttributes(CVertexArray* _pVertexArray, CVertexBuffer<stVertex>* _pVertexBuffer, CElementBuffer* _pElementBuffer)
	{
		if (_pVertexArray == nullptr || _pVertexBuffer == nullptr || _pElementBuffer == nullptr)
		{

			return;
		}

		_pVertexArray->Bind(); _pVertexBuffer->Bind(); _pElementBuffer->Bind();

		_pVertexArray->LinkAttribute(0 /*m_v3Position*/, 3, GL_FLOAT, sizeof(stVertex), (void*)0);
		_pVertexArray->LinkAttribute(1 /*m_v3Normal*/, 3, GL_FLOAT, sizeof(stVertex), (void*)(3 * sizeof(float)));
		_pVertexArray->LinkAttribute(2 /*m_v2TextureCoord*/, 2, GL_FLOAT, sizeof(stVertex), (void*)(6 * sizeof(float)));
		_pVertexArray->LinkAttribute(3 /*m_v3Color*/, 3, GL_FLOAT, sizeof(stVertex), (void*)(8 * sizeof(float)));

		_pVertexArray->Unbind(); _pVertexBuffer->Unbind(); _pElementBuffer->Unbind();
	}
};

class CMesh
{
protected:
	CVertexArray m_VertexArray;
	CElementBuffer m_ElementBuffer;
	CVertexBuffer<stVertex> m_VertexBuffer;

	bool m_bUpdateVertexArray;
	virtual void UpdateVertexArray();

public:
	std::vector <CTexture*> m_vTextures;
	CShader* m_pShader;

	CMesh();
	CMesh(std::vector<stVertex>& _vVerticies, std::vector<unsigned int>& _vIndicies, std::vector<CTexture*>& _Textures, CShader* _Shader);
	~CMesh() {};

	const std::vector<stVertex> GetVerticies() const;
	void SetVerticies(const std::vector<stVertex> _vVerticies);
	const std::vector<unsigned int> GetIndicies() const;
	void SetIndicies(const std::vector<unsigned int> _vIndicies);

	void Draw(const CCamera& _Camera);
};