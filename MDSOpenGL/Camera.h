#pragma once

#include <GLEW/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include"Shader.h"

class CCamera
{
public:
	glm::vec3 m_v3Position;
	glm::vec3 m_v3Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 m_v3Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 m_mat4CameraMatrix = glm::mat4(1.0f);

	bool m_bFirstClick = true;

	unsigned int m_uViewPortW, m_uViewPortH;

	float m_fSpeed = 0.1f * 0.001f;
	float m_fSensitivity = 100.0f;

	CCamera(int _iViewPortW, int _iViewPortH, glm::vec3 _v3Position);

	void UpdateMatrix(float _fFOVdeg, float _fNearPlane, float _fFarPlane);
	void Matrix(CShader& _Shader, const char* _pUniform);
	void Inputs(GLFWwindow* _Window);
};
