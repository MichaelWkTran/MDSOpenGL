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
private:
	const glm::vec3 m_v3Up = glm::vec3(0.0f, 1.0f, 0.0f);

	unsigned int* m_pViewPortW;
	unsigned int* m_pViewPortH;

	bool m_IsPerspective;
	float m_fFOV; //Value is in degrees
	float m_fOrthographicScale;
	float m_fNearPlane, m_fFarPlane;

	glm::mat4 m_mat4View, m_mat4Projection, m_mat4Camera;


	void UpdateProjectionMatrix();

public:
	glm::vec3 m_v3Position;
	glm::vec3 m_v3Orientation;

	bool m_bFirstClick = true;
	float m_fSpeed = 0.1f * 0.001f;
	float m_fSensitivity = 100.0f;

	CCamera
	(
		unsigned int* _pViewPortW,
		unsigned int* _pViewPortH,
		bool _IsPerspective,
		glm::vec3 _v3Position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 _v3Orientation = glm::vec3(0.0f, 0.0f, 1.0f),
		float _fFOVorScale = 0.0f,
		float _fNearPlane = 0.1f,
		float _fFarPlane = 100.0f
	);

	void Update();
	void Inputs(GLFWwindow* _Window);

	//Get Set Methods
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetCameraMatrix();
	float GetFOV(bool _InRadians = false); void SetFOV(float _FOV, bool _IsRadians = false);
	float GetNearPlane(); void SetNearPlane(float _fNearPlane);
	float GetFarPlane(); void SetFarPlane(float _fFarPlane);
};
