#include "Mesh.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

void CMesh::UpdateVertexArray()
{
	if (!m_bUpdateVertexArray || m_pShader == nullptr) return void();
	m_bUpdateVertexArray = false;

	stVertex::LinkAttributes(&m_VertexArray, &m_VertexBuffer, &m_ElementBuffer);
}

CMesh::CMesh()
{
	m_pShader = nullptr;
	m_bUpdateVertexArray = false;
};

CMesh::CMesh(std::vector<stVertex>& _vVerticies, std::vector<unsigned int>& _vIndicies, std::vector<CTexture*>& _vTextures, CShader* _Shader)
{
	m_VertexBuffer.SetVertices(_vVerticies);
	m_ElementBuffer.SetIndicies(_vIndicies);
	m_vTextures = _vTextures;
	m_pShader = _Shader;

	m_bUpdateVertexArray = true;
}

const std::vector<stVertex> CMesh::GetVerticies() const
{
	return m_VertexBuffer.GetVertices();
}

void CMesh::SetVerticies(const std::vector<stVertex> _vVerticies)
{
	m_VertexBuffer.SetVertices(_vVerticies);
	m_bUpdateVertexArray = true;
}

const std::vector<unsigned int> CMesh::GetIndicies() const
{
	return m_ElementBuffer.GetIndicies();
}

void CMesh::SetIndicies(const std::vector<unsigned int> _vIndicies)
{
	m_ElementBuffer.SetIndicies(_vIndicies);
	m_bUpdateVertexArray = true;
}

void CMesh::Draw(const CCamera& _Camera)
{
	if (m_pShader == nullptr || m_VertexBuffer.GetVertices().empty()) return void();

	//Update vertex array if its transform has changed
	UpdateVertexArray();

	//Bind vertex array and activate shader
	m_VertexArray.Bind();
	m_pShader->Activate();

	//Set Mesh Uniforms
	m_pShader->Uniform3f("uni_v3CameraPosition", _Camera.m_v3Position);
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