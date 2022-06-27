#include "Mesh.h"
#include <glm/gtx/vector_angle.hpp>

#pragma once
class CPlayer
{
private:
	CMesh m_Mesh;
	glm::mat4x4 m_mat4Model;

	bool m_bUpdateTransform;
	glm::vec3 m_v3Position;
	glm::fquat m_fquatRotation;
	glm::vec3 m_v3Scale;

public:
	CPlayer();
	
	const CShader* GetShader() const; void SetShader(CShader* _Shader);
	const glm::vec3& GetPosition() const; void SetPosition(const glm::vec3 _v3Position);
	const glm::fquat& GetRotation() const; void SetRotation(const glm::fquat _fquatRotation);
	const glm::vec3& GetScale() const; void SetScale(const glm::vec3 _v3Scale);

	void Input(GLFWwindow* _pWindow);
	void Draw(CCamera& _Camera);
};