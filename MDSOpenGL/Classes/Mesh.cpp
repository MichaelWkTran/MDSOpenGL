#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

template class CMesh<stVertex>;