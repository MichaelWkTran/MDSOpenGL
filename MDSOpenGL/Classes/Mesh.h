#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include "UpdatedObject.h"
#include "Transform.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"

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
	std::vector <CTexture*> m_vTextures;
	CShader* m_pShader;

	CMesh();
	CMesh(std::vector<T>& _vVerticies, std::vector<unsigned int>& _vIndicies, std::vector<CTexture*>& _Textures, CShader* _Shader);
	~CMesh() {};

	const std::vector<T> GetVerticies() const;
	void SetVerticies(const std::vector<T> _vVerticies);
	const std::vector<unsigned int> GetIndicies() const;
	void SetIndicies(const std::vector<unsigned int> _vIndicies);

	void Draw(const CCamera& _Camera);
};

template<class T>
inline CMesh<T>::CMesh()
{
	m_pShader = nullptr;
	m_bUpdateVertexArray = false;
}

template<class T>
inline CMesh<T>::CMesh(std::vector<T>& _vVerticies, std::vector<unsigned int>& _vIndicies, std::vector<CTexture*>& _vTextures, CShader* _Shader)
{
	m_VertexBuffer.SetVertices(_vVerticies);
	m_ElementBuffer.SetIndicies(_vIndicies);
	m_vTextures = _vTextures;
	m_pShader = _Shader;

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

template<class T>
inline void CMesh<T>::Draw(const CCamera& _Camera)
{
	if (m_pShader == nullptr || m_VertexBuffer.GetVertices().empty()) return void();

	//Update vertex array if its vertex data has changed
	if (m_bUpdateVertexArray)
	{
		m_bUpdateVertexArray = false;
		T::LinkAttributes(&m_VertexArray, &m_VertexBuffer, &m_ElementBuffer);
	}

	//Bind vertex array and activate shader
	m_VertexArray.Bind();
	m_pShader->Activate();

	//Set GameObject Uniform
	if (m_pTransform != nullptr)
	{
		m_pTransform->Draw(_Camera);
		glUniformMatrix4fv(glGetUniformLocation(*m_pShader, "uni_mat4Model"), 1, GL_FALSE, glm::value_ptr(m_pTransform->GetModel()));
	}
	else
	{
		glUniformMatrix4fv(glGetUniformLocation(*m_pShader, "uni_mat4Model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));
	}
	
	//Set Mesh Uniforms
	m_pShader->Uniform3f("uni_v3CameraPosition", _Camera.m_Transform.GetPosition());
	glUniformMatrix4fv(glGetUniformLocation(*m_pShader, "uni_mat4CameraMatrix"), 1, GL_FALSE, glm::value_ptr(_Camera.GetCameraMatrix()));

	//Set Texture Uniforms
	unsigned int uNumDiffuse = 0, uNumSpecular = 0, uNumReflect = 0;
	for (auto& pTexture : m_vTextures)
	{
		//Get the name of the uniform in the format: uni_samp + "texture target" + "texture type" + "add array index"
		std::string strUniformName = pTexture->m_pType;

		//Add array number
		if (pTexture->m_pType == "Diffuse") strUniformName += std::to_string(uNumDiffuse++);
		else if (pTexture->m_pType == "Specular") strUniformName += std::to_string(uNumSpecular++);
		else if (pTexture->m_pType == "Reflect") strUniformName += std::to_string(uNumReflect++);
		else continue;

		//Set Uniform
		pTexture->Uniform(*m_pShader, strUniformName);
	}
	CTextureManager::Unbind();

	//Draw Mesh
	glDrawElements(GL_TRIANGLES, m_ElementBuffer.GetIndicies().size(), GL_UNSIGNED_INT, 0);

	//Deactivate and unbind vertex array and shader
	m_pShader->Deactivate();
	m_VertexArray.Unbind();
}