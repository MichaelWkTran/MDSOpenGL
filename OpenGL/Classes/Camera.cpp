#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "../ExternVariables.h"

CCamera::CCamera
(
	unsigned int _uViewPortW,
	unsigned int _uViewPortH,
	bool _IsPerspective,
	glm::vec3 _v3Position/* = glm::vec3(0.0f, 0.0f, 0.0f)*/,
	glm::vec3 _v3Orientation/* = glm::vec3(0.0f, 0.0f, 1.0f)*/,
	float _fFOVorScale/* = 0.0f*/,
	float _fNearPlane/* = 0.1f*/, float _fFarPlane/* = 100.0f*/
)
{
	m_uViewPortW = _uViewPortW;
	m_uViewPortH = _uViewPortH;

	m_IsPerspective = _IsPerspective;
	m_v3Position = _v3Position;
	m_v3Orientation = _v3Orientation;
	if (_fFOVorScale == 0.0f)
	{
		m_fFOV = 45.0f; m_fOrthographicScale = 1;
	}
	else
	{
		m_fFOV = 45.0f; m_fOrthographicScale = 1;

		if (_IsPerspective) { m_fFOV = _fFOVorScale; }
		else { m_fOrthographicScale = _fFOVorScale; }
	}
	m_fNearPlane = _fNearPlane;
	m_fFarPlane = _fFarPlane;

	m_mat4View = glm::mat4(1.0f);
	m_mat4Projection = glm::mat4(1.0f);
	m_mat4Camera = glm::mat4(1.0f);

	UpdateProjectionMatrix();
	Update();
}

void CCamera::UpdateProjectionMatrix()
{
	if (m_IsPerspective)
	{
		m_mat4Projection = glm::perspective(glm::radians(m_fFOV), ((float)m_uViewPortW) / ((float)m_uViewPortH), m_fNearPlane, m_fFarPlane);
	}
	else
	{
		float fHalfViewPortW = (((float)m_uViewPortW) / 2) / m_fOrthographicScale;
		float fHalfViewPortH = (((float)m_uViewPortH) / 2) / m_fOrthographicScale;

		m_mat4Projection = glm::ortho(fHalfViewPortW, -fHalfViewPortW, fHalfViewPortH, -fHalfViewPortH, m_fNearPlane, m_fFarPlane);
	}
}

const glm::mat4 CCamera::GetViewMatrix() const
{
	return m_mat4View;
}

const glm::mat4 CCamera::GetProjectionMatrix() const
{
	return m_mat4Projection;
}

const glm::mat4 CCamera::GetCameraMatrix() const
{
	return m_mat4Camera;
}

const float CCamera::GetFOV(bool _InRadians/* = false*/)
{
	if (_InRadians) return glm::radians(m_fFOV);
	else return m_fFOV;
}

void CCamera::SetFOV(float _FOV, bool _IsRadians/* = false*/)
{
	m_fFOV = _IsRadians ? glm::degrees(_FOV) : _FOV;

	UpdateProjectionMatrix();
}

const float CCamera::GetNearPlane() const
{
	return m_fNearPlane;
}

void CCamera::SetNearPlane(const float _fNearPlane)
{
	m_fNearPlane = _fNearPlane;

	UpdateProjectionMatrix();
}

const float CCamera::GetFarPlane() const
{
	return m_fFarPlane;
}

void CCamera::SetFarPlane(const float _fFarPlane)
{
	m_fFarPlane = _fFarPlane;

	UpdateProjectionMatrix();
}

void CCamera::Inputs(GLFWwindow* _pWindow)
{
	float m_fSpeed = 1.0f;
	float m_fSensitivity = 30.0f;
	glm::vec3 m_v3Up = glm::vec3(0.0f, 1.0f, 0.0f);

	//Handles key inputs
	if (glfwGetKey(_pWindow, GLFW_KEY_W) == GLFW_PRESS) m_v3Position += m_fSpeed * m_v3Orientation * e_fDeltatime;
	if (glfwGetKey(_pWindow, GLFW_KEY_S) == GLFW_PRESS) m_v3Position += m_fSpeed * -m_v3Orientation * e_fDeltatime;
	if (glfwGetKey(_pWindow, GLFW_KEY_A) == GLFW_PRESS) m_v3Position += m_fSpeed * -glm::normalize(glm::cross(m_v3Orientation, m_v3Up)) * e_fDeltatime;
	if (glfwGetKey(_pWindow, GLFW_KEY_D) == GLFW_PRESS) m_v3Position += m_fSpeed * glm::normalize(glm::cross(m_v3Orientation, m_v3Up)) * e_fDeltatime;
	if (glfwGetKey(_pWindow, GLFW_KEY_SPACE) == GLFW_PRESS) m_v3Position += m_fSpeed * m_v3Up * e_fDeltatime;
	if (glfwGetKey(_pWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) m_v3Position += m_fSpeed * -m_v3Up * e_fDeltatime;

	//Handles mouse inputs
	if (glfwGetMouseButton(_pWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		//Hides mouse cursor
		glfwSetInputMode(_pWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		//Prevents camera from jumping on the first click
		if (m_bFirstClick)
		{
			glfwSetCursorPos(_pWindow, (e_uViewPortW / 2), (e_uViewPortH / 2));
			e_v2MousePosition = glm::vec2(e_uViewPortW/2, e_uViewPortH/2);
			m_bFirstClick = false;
		}

		//Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		//and then "transforms" them into degrees 
		float fRotX = m_fSensitivity * (e_v2MousePosition.x - (e_uViewPortW/2)) * e_fDeltatime;
		float fRotY = m_fSensitivity * (e_v2MousePosition.y - (e_uViewPortH/2)) * e_fDeltatime;


		//Calculates upcoming vertical change in the Orientation
		glm::vec3 v3NewOrientation = glm::rotate(m_v3Orientation, glm::radians(-fRotY), glm::normalize(glm::cross(m_v3Orientation, m_v3Up)));

		//Decides whether or not the next vertical Orientation is legal or not
		if (abs(glm::angle(v3NewOrientation, m_v3Up) - glm::radians(90.0f)) <= glm::radians(85.0f)) m_v3Orientation = v3NewOrientation;
		
		//Rotates the Orientation left and right
		m_v3Orientation = glm::rotate(m_v3Orientation, glm::radians(-fRotX), m_v3Up);

		//Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(_pWindow, (e_uViewPortW / 2), (e_uViewPortH / 2));
		e_v2MousePosition = glm::vec2(e_uViewPortW / 2, e_uViewPortH / 2);
	}
	else if (glfwGetMouseButton(_pWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		//Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(_pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		//Makes sure the next time the camera looks around it doesn't jump
		m_bFirstClick = true;
	}
}

void CCamera::Update()
{
	m_mat4View = glm::lookAt(m_v3Position, m_v3Position + m_v3Orientation, glm::vec3(0.0f, 1.0f, 0.0f));
	m_mat4Camera = m_mat4Projection * m_mat4View;
}