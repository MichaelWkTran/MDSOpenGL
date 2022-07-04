#pragma once
#include <glm/gtx/vector_angle.hpp>
#include <vector>
#include "UpdatedObject.h"

class CGameManager;
class CCamera;

class CTransform : public CUpdatedObject
{
private:
	bool m_bUpdateTransform;
	glm::vec3 m_v3Position;
	glm::fquat m_fquatRotation;
	glm::vec3 m_v3Scale;
	glm::mat4x4 m_mat4Model;

	bool m_bDeleteGameObject;

public:
	CTransform();
	~CTransform() {};

	const glm::mat4x4 GetModel() const;
	const glm::vec3 GetPosition() const; void SetPosition(const glm::vec3 _v3Position);
	const glm::fquat GetRotation() const; void SetRotation(const glm::fquat _fquatRotation);
	const glm::vec3 GetRotationEuler(bool _InDegrees = true) const; void SetRotationEuler(const glm::vec3 _v3Rotation, bool _InDegrees = true);
	const glm::vec3 GetScale() const; void SetScale(const glm::vec3 _v3Scale);

	const glm::vec3 Up() const;
	const glm::vec3 Forward() const;
	const glm::vec3 Right() const;

	void Draw(const CCamera& _Camera);
};