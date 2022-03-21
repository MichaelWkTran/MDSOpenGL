#include "Mesh.h"

CMesh::CMesh(std::vector<stVertex>& _vVerticies, std::vector<GLuint>& _vIndicies, std::vector<CTexture>& _vTextures)
{
	m_vVerticies = _vVerticies;
	m_vIndicies = _vIndicies;
	m_vTextures = _vTextures;

	m_VertexArray.Bind();
	CVertexBuffer VertexBuffer(_vVerticies);
	CElementBuffer ElementBuffer(_vIndicies);

	m_VertexArray.LinkAttribute(VertexBuffer, 0 /*m_v3CurrentPosition*/, 3, GL_FLOAT, sizeof(stVertex), (void*)0);
	m_VertexArray.LinkAttribute(VertexBuffer, 1 /*m_v3Normal*/, 3, GL_FLOAT, sizeof(stVertex), (void*)(3 * sizeof(float)));
	m_VertexArray.LinkAttribute(VertexBuffer, 2 /*m_v3Color*/, 3, GL_FLOAT, sizeof(stVertex), (void*)(6 * sizeof(float)));
	m_VertexArray.LinkAttribute(VertexBuffer, 3 /*m_v2TextureCoord*/, 2, GL_FLOAT, sizeof(stVertex), (void*)(9 * sizeof(float)));

	m_VertexArray.Unbind(); VertexBuffer.Unbind(); ElementBuffer.Unbind();
}

void CMesh::Draw(CShader& _Shader, CCamera& _Camera)
{
	_Shader.Activate();
	m_VertexArray.Bind();

	unsigned int uNumDiffuse = 0;
	unsigned int uNumSpecular = 0;
	for (unsigned int i = 0; i < m_vTextures.size(); i++)
	{
		std::string strNum;
		std::string strType = m_vTextures[i].m_pType;

		if (strType == "Diffuse") { strNum = std::to_string(uNumDiffuse++); }
		else if (strType == "Specular") { strNum = std::to_string(uNumSpecular++); }

		m_vTextures[i].TextureUnit(_Shader, ("uni_samp2D" + strType + '[' + strNum + ']').c_str(), i);
		m_vTextures[i].Bind();
	}

	glUniform3f(glGetUniformLocation(_Shader.GetID(), "uni_v3CameraPosition"), _Camera.m_v3Position.x, _Camera.m_v3Position.y, _Camera.m_v3Position.z);
	glUniformMatrix4fv(glGetUniformLocation(_Shader.GetID(), "uni_mat4CameraMatrix"), 1, GL_FALSE, glm::value_ptr(_Camera.GetCameraMatrix()));

	glDrawElements(GL_TRIANGLES, m_vIndicies.size(), GL_UNSIGNED_INT, 0);
}
