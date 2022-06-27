#pragma once
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class CCamera
{
protected:
	bool m_IsPerspective;
	float m_fFOV; //Value is in degrees
	float m_fOrthographicScale;
	float m_fNearPlane, m_fFarPlane;

	glm::mat4 m_mat4View, m_mat4Projection, m_mat4Camera;

	void UpdateProjectionMatrix();

public:
	unsigned int m_uViewPortW, m_uViewPortH;
	glm::vec3 m_v3Position;
	glm::vec3 m_v3Orientation;
	bool m_bFirstClick = true;

	CCamera
	(
		unsigned int _uViewPortW,
		unsigned int _uViewPortH,
		bool _IsPerspective,
		glm::vec3 _v3Position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 _v3Orientation = glm::vec3(0.0f, 0.0f, 1.0f),
		float _fFOVorScale = 0.0f,
		float _fNearPlane = 0.1f,
		float _fFarPlane = 100.0f
	);
	~CCamera() {}

	const glm::mat4 GetViewMatrix() const;
	const glm::mat4 GetProjectionMatrix() const;
	const glm::mat4 GetCameraMatrix() const;
	const float GetFOV(bool _InRadians = false); void SetFOV(float _FOV, bool _IsRadians = false);
	const float GetNearPlane() const; void SetNearPlane(const float _fNearPlane);
	const float GetFarPlane() const; void SetFarPlane(const float _fFarPlane);

	void Update();
};
