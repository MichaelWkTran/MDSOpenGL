#include "Camera.h"

CCamera::CCamera
(
	unsigned int* _pViewPortW,
	unsigned int* _pViewPortH,
	bool _IsPerspective,
	glm::vec3 _v3Position/* = glm::vec3(0.0f, 0.0f, 0.0f)*/,
	glm::vec3 _v3Orientation/* = glm::vec3(0.0f, 0.0f, 1.0f)*/,
	float _fFOVorScale/* = 0.0f*/,
	float _fNearPlane/* = 0.1f*/, float _fFarPlane/* = 100.0f*/
)
{
	m_pViewPortW = _pViewPortW;
	m_pViewPortH = _pViewPortH;

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
	CCamera::Update();
}

void CCamera::UpdateProjectionMatrix()
{
	if (m_IsPerspective)
	{
		m_mat4Projection = glm::perspective(glm::radians(m_fFOV), ((float)(*m_pViewPortW)) / ((float)(*m_pViewPortH)), m_fNearPlane, m_fFarPlane);
	}
	else
	{
		float fHalfViewPortW = (((float)(*m_pViewPortW)) / 2) / m_fOrthographicScale;
		float fHalfViewPortH = (((float)(*m_pViewPortH)) / 2) / m_fOrthographicScale;

		m_mat4Projection = glm::ortho(fHalfViewPortW, -fHalfViewPortW, fHalfViewPortH, -fHalfViewPortH, m_fNearPlane, m_fFarPlane);
	}
}

void CCamera::Update()
{
	m_mat4View = glm::lookAt(m_v3Position, m_v3Position + m_v3Orientation, m_v3Up);
	m_mat4Camera = m_mat4Projection * m_mat4View;
}

//Get Set Methods
glm::mat4  CCamera::GetViewMatrix()
{
	return m_mat4View;
}

glm::mat4  CCamera::GetProjectionMatrix()
{
	return m_mat4Projection;
}

glm::mat4  CCamera::GetCameraMatrix()
{
	return m_mat4Camera;
}

float CCamera::GetFOV(bool _InRadians/* = false*/)
{
	if (_InRadians) return glm::radians(m_fFOV);
	else return m_fFOV;
}

void CCamera::SetFOV(float _FOV, bool _IsRadians/* = false*/)
{
	m_fFOV = _IsRadians ? glm::degrees(_FOV) : _FOV;

	UpdateProjectionMatrix();
}

float CCamera::GetNearPlane()
{
	return m_fNearPlane;
}

void CCamera::SetNearPlane(float _fNearPlane)
{
	m_fNearPlane = _fNearPlane;

	UpdateProjectionMatrix();
}

float CCamera::GetFarPlane()
{
	return m_fFarPlane;
}

void CCamera::SetFarPlane(float _fFarPlane)
{
	m_fFarPlane = _fFarPlane;

	UpdateProjectionMatrix();
}

void CCamera::Inputs(GLFWwindow* _Window)
{
	// Handles key inputs
	if (glfwGetKey(_Window, GLFW_KEY_W) == GLFW_PRESS)
	{
		m_v3Position += m_fSpeed * m_v3Orientation;
	}
	if (glfwGetKey(_Window, GLFW_KEY_A) == GLFW_PRESS)
	{
		m_v3Position += m_fSpeed * -glm::normalize(glm::cross(m_v3Orientation, m_v3Up));
	}
	if (glfwGetKey(_Window, GLFW_KEY_S) == GLFW_PRESS)
	{
		m_v3Position += m_fSpeed * -m_v3Orientation;
	}
	if (glfwGetKey(_Window, GLFW_KEY_D) == GLFW_PRESS)
	{
		m_v3Position += m_fSpeed * glm::normalize(glm::cross(m_v3Orientation, m_v3Up));
	}
	if (glfwGetKey(_Window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		m_v3Position += m_fSpeed * m_v3Up;
	}
	if (glfwGetKey(_Window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		m_v3Position += m_fSpeed * -m_v3Up;
	}
	if (glfwGetKey(_Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		m_fSpeed = 0.4f * 0.01f;
	}
	else if (glfwGetKey(_Window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		m_fSpeed = 0.1f * 0.01f;
	}


	// Handles mouse inputs
	if (glfwGetMouseButton(_Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Hides mouse cursor
		glfwSetInputMode(_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (m_bFirstClick)
		{
			glfwSetCursorPos(_Window, ((*m_pViewPortW) / 2), ((*m_pViewPortH) / 2));
			m_bFirstClick = false;
		}

		// Stores the coordinates of the cursor
		double dMouseX;
		double dMouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(_Window, &dMouseX, &dMouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float fRotX = m_fSensitivity * (float)(dMouseY - ((*m_pViewPortH) / 2)) / (*m_pViewPortH);
		float fRotY = m_fSensitivity * (float)(dMouseX - ((*m_pViewPortW) / 2)) / (*m_pViewPortW);

		// Calculates upcoming vertical change in the Orientation
		glm::vec3 newOrientation = glm::rotate(m_v3Orientation, glm::radians(-fRotX), glm::normalize(glm::cross(m_v3Orientation, m_v3Up)));

		// Decides whether or not the next vertical Orientation is legal or not
		if (abs(glm::angle(newOrientation, m_v3Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			m_v3Orientation = newOrientation;
		}

		// Rotates the Orientation left and right
		m_v3Orientation = glm::rotate(m_v3Orientation, glm::radians(-fRotY), m_v3Up);

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(_Window, ((*m_pViewPortW) / 2), ((*m_pViewPortH) / 2));
	}
	else if (glfwGetMouseButton(_Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		m_bFirstClick = true;
	}
}