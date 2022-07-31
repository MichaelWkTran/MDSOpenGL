#pragma once
#include "UpdatedObject.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "Shader.h"
#include "Camera.h"

class CTransform;
class CTexture;
class CShader;

struct stVertex
{
	glm::vec3 v3Position;
	glm::vec3 v3Normal;
	glm::vec2 v2TextureUV;
	glm::vec3 v3Colour = glm::vec3(1, 1, 1);

	static void LinkAttributes(CVertexArray* _pVertexArray, CVertexBuffer<stVertex>* _pVertexBuffer, CElementBuffer* _pElementBuffer)
	{
		if (_pVertexArray == nullptr || _pVertexBuffer == nullptr || _pElementBuffer == nullptr) return;

		_pVertexArray->Bind(); _pVertexBuffer->Bind(); _pElementBuffer->Bind();

		_pVertexArray->LinkAttribute(0 /*m_v3Position*/, 3, GL_FLOAT, sizeof(stVertex), (void*)0);
		_pVertexArray->LinkAttribute(1 /*m_v3Normal*/, 3, GL_FLOAT, sizeof(stVertex), (void*)(3 * sizeof(float)));
		_pVertexArray->LinkAttribute(2 /*m_v2TextureCoord*/, 2, GL_FLOAT, sizeof(stVertex), (void*)(6 * sizeof(float)));
		_pVertexArray->LinkAttribute(3 /*m_v3Color*/, 3, GL_FLOAT, sizeof(stVertex), (void*)(8 * sizeof(float)));

		_pVertexArray->Unbind(); _pVertexBuffer->Unbind(); _pElementBuffer->Unbind();
	}
};

template <class T = stVertex>
class CMesh : public CUpdatedObject
{
protected:
	CVertexArray m_VertexArray;
	CElementBuffer m_ElementBuffer;
	CVertexBuffer<T> m_VertexBuffer;
	bool m_bUpdateVertexArray;

public:
	CTransform* m_pTransform;
	std::vector<CTexture*> m_vTextures;
	CShader* m_pShader;

	CMesh();
	CMesh(std::vector<T>& _vVerticies, std::vector<unsigned int>& _vIndicies, std::vector<CTexture*>& _vTextures, CShader* _pShader);
	~CMesh() {};

	const std::vector<T> GetVerticies() const;
	void SetVerticies(const std::vector<T> _vVerticies);
	const std::vector<unsigned int> GetIndicies() const;
	void SetIndicies(const std::vector<unsigned int> _vIndicies);

	virtual void Draw(const CCamera& _Camera) override;
};

template<class T>
inline CMesh<T>::CMesh()
{
	m_pTransform = nullptr;
	m_pShader = nullptr;
	m_bUpdateVertexArray = false;
}

template<class T>
inline CMesh<T>::CMesh(std::vector<T>& _vVerticies, std::vector<unsigned int>& _vIndicies, std::vector<CTexture*>& _vTextures, CShader* _pShader)
{
	m_VertexBuffer.SetVertices(_vVerticies);
	m_ElementBuffer.SetIndicies(_vIndicies);
	m_vTextures = _vTextures;
	m_pShader = _pShader;

	m_bUpdateVertexArray = true;
}

template<class T>
inline const std::vector<T> CMesh<T>::GetVerticies() const
{
	return m_VertexBuffer.GetVertices();
}

template<class T>
inline void CMesh<T>::SetVerticies(const std::vector<T> _vVerticies)
{
	m_VertexBuffer.SetVertices(_vVerticies);
	m_bUpdateVertexArray = true;
}

template<class T>
inline const std::vector<unsigned int> CMesh<T>::GetIndicies() const
{
	return m_ElementBuffer.GetIndicies();
}

template<class T>
inline void CMesh<T>::SetIndicies(const std::vector<unsigned int> _vIndicies)
{
	m_ElementBuffer.SetIndicies(_vIndicies);
	m_bUpdateVertexArray = true;
}