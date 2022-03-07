#include "Camera.h"

CCamera::CCamera(int _iViewPortW, int _iViewPortH, glm::vec3 _v3Position)
{
	m_uViewPortW = _iViewPortW;
	m_uViewPortH = _iViewPortH;
	m_v3Position = _v3Position;
}

void CCamera::UpdateMatrix(float _fFOVdeg, float _fNearPlane, float _fFarPlane)
{
	glm::mat4 mat4View = glm::mat4(1.0f);
	glm::mat4 mat4Projection = glm::mat4(1.0f);

	mat4View = glm::lookAt(m_v3Position, m_v3Position + m_v3Orientation, m_v3Up);
	mat4Projection = glm::perspective(glm::radians(_fFOVdeg), (float)m_uViewPortW / (float)m_uViewPortH, _fNearPlane, _fFarPlane);

	m_mat4CameraMatrix = mat4Projection * mat4View;
}

void CCamera::Matrix(CShader& _Shader, const char* _pUniform)
{
	glUniformMatrix4fv(glGetUniformLocation(_Shader.m_GLuID, _pUniform), 1, GL_FALSE, glm::value_ptr(m_mat4CameraMatrix));
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
			glfwSetCursorPos(_Window, (m_uViewPortW / 2), (m_uViewPortH / 2));
			m_bFirstClick = false;
		}
	
		// Stores the coordinates of the cursor
		double dMouseX;
		double dMouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(_Window, &dMouseX, &dMouseY);
	
		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float fRotX = m_fSensitivity * (float)(dMouseY - (m_uViewPortH / 2)) / m_uViewPortH;
		float fRotY = m_fSensitivity * (float)(dMouseX - (m_uViewPortW / 2)) / m_uViewPortW;
	
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
		glfwSetCursorPos(_Window, (m_uViewPortW / 2), (m_uViewPortH / 2));
	}
	else if (glfwGetMouseButton(_Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		m_bFirstClick = true;
	}
}