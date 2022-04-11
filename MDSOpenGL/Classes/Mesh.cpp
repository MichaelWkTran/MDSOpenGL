#include "Mesh.h"

void CMesh::UpdateVertexArray()
{
	if (!m_bUpdateVertexArray) return void();
	m_bUpdateVertexArray = false;

	m_VertexArray.Bind();
	CVertexBuffer VertexBuffer(m_vVerticies);
	CElementBuffer ElementBuffer(m_vIndicies);

	m_VertexArray.LinkAttribute(VertexBuffer, 0 /*m_v3CurrentPosition*/, 3, GL_FLOAT, sizeof(stVertex), (void*)0);
	m_VertexArray.LinkAttribute(VertexBuffer, 1 /*m_v3Normal*/, 3, GL_FLOAT, sizeof(stVertex), (void*)(3 * sizeof(float)));
	m_VertexArray.LinkAttribute(VertexBuffer, 2 /*m_v3Color*/, 3, GL_FLOAT, sizeof(stVertex), (void*)(6 * sizeof(float)));
	m_VertexArray.LinkAttribute(VertexBuffer, 3 /*m_v2TextureCoord*/, 2, GL_FLOAT, sizeof(stVertex), (void*)(9 * sizeof(float)));

	m_VertexArray.Unbind(); VertexBuffer.Unbind(); ElementBuffer.Unbind();
}

CMesh::CMesh(std::vector<stVertex>& _vVerticies, std::vector<GLuint>& _vIndicies, std::vector<CTexture>& _vTextures, CShader* _pShader)
{
	m_vVerticies = _vVerticies;
	m_vIndicies = _vIndicies;
	m_vTextures = _vTextures;
	m_pShader = _pShader;

	m_bUpdateVertexArray = true;
}

std::vector<stVertex> CMesh::GetVerticies() const
{
	return std::vector<stVertex>();
}

void CMesh::SetVerticies(std::vector<stVertex>& _vVerticies)
{
	m_vVerticies = _vVerticies;
	m_bUpdateVertexArray = true;
}

std::vector<GLuint> CMesh::GetIndicies() const
{
	return std::vector<GLuint>();
}

void CMesh::SetIndicies(std::vector<GLuint>& _vIndicies)
{
	m_vIndicies = _vIndicies;
	m_bUpdateVertexArray = true;
}

void CMesh::Draw(CCamera& _Camera)
{
	UpdateVertexArray();

	GLint GluCurrentProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &GluCurrentProgram);
	if (GluCurrentProgram != m_pShader->GetID()) m_pShader->Activate();
	m_VertexArray.Bind();

	unsigned int uNumDiffuse = 0;
	unsigned int uNumSpecular = 0;
	for (unsigned int i = 0; i < m_vTextures.size(); i++)
	{
		std::string strNum;
		std::string strType = m_vTextures[i].m_pType;

		if (strType == "Diffuse") { strNum = std::to_string(uNumDiffuse++); }
		else if (strType == "Specular") { strNum = std::to_string(uNumSpecular++); }

		m_vTextures[i].TextureUnit(*m_pShader, ("uni_samp2D" + strType + '[' + strNum + ']').c_str(), i);
		m_vTextures[i].Bind();
	}

	glUniform3f(glGetUniformLocation(m_pShader->GetID(), "uni_v3CameraPosition"), _Camera.m_v3Position.x, _Camera.m_v3Position.y, _Camera.m_v3Position.z);
	glUniformMatrix4fv(glGetUniformLocation(m_pShader->GetID(), "uni_mat4CameraMatrix"), 1, GL_FALSE, glm::value_ptr(_Camera.GetCameraMatrix()));

	glDrawElements(GL_TRIANGLES, m_vIndicies.size(), GL_UNSIGNED_INT, 0);
}
